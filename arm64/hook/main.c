#include "ptrace_util.h"
#include <signal.h>

// 采用信号控制当前程序的流程
static int signal_command;

// 保存一条 aarch64 指令的数据结构
union OneInstruction
{
	uint32_t instruction;
	uint8_t bytes[4];
};
// aarch64 illegal instruction: 0xe7fXXXfX
const static uint32_t illegal_instruction = 0xe7f000f0; // aarch64 的异常指令 0xe7f000f0

struct Hook_point_info
{
	char func_name[0x100]; // 被hook函数名

	unsigned long entry_addr;			   // 待hook 函数在进程中的虚拟地址
	union OneInstruction entry_point_code; // 函数开始处的原来数据(保存后便于以后还原)
	struct pt_regs entry_regs;			   // 程序执行到函数开始处的寄存器信息

	unsigned long back_addr;			  // 函数执行完，需要返回的地址.这里下断点为了取返回值
	union OneInstruction back_point_code; // 函数执行完返回处的原来数据(保存后便于以后还原)
	struct pt_regs back_regs;			  // 程序执行到函数返回地址处的寄存器信息
};

// 下断点并保存此位置原来数据 (aarch64 往目标地址写入异常指令)
int set_breakpoint(pid_t pid, unsigned long addr, union OneInstruction *oneInstruction)
{
	// save context 保存断点处数据
	getdata(pid, addr, oneInstruction->bytes, 4);

	// 下断点
	union OneInstruction illegal_instruction;
	// aarch64 illegal instruction: 0xe7fXXXfX
	illegal_instruction.instruction = 0xe7f000f0;
	putdata(pid, addr, illegal_instruction.bytes, 4);

	return 0;
}

// 进入函数
int onEnter(pid_t pid, struct pt_regs *regs, struct Hook_point_info *hook_point)
{

	// 保存函数相关信息
	memcpy(&(hook_point->entry_regs), &regs, sizeof(struct pt_regs));
	hook_point->back_addr = regs->ARM_lr;

	// 移除断点，即恢复程序上下文信息
	putdata(pid, hook_point->entry_addr, hook_point->entry_point_code.bytes, 4);
	// set_registers(pid, &(hook_point->entry_regs));

	//并把函数返回地址设置成断点，便于获取函数返回值
	set_breakpoint(pid, hook_point->back_addr, &(hook_point->back_point_code));

	return 0;
}

// 离开函数
int onLeave(pid_t pid, struct pt_regs *regs, struct Hook_point_info *hook_point)
{
	// 保存函数相关信息
	memcpy(&(hook_point->back_regs), &regs, sizeof(struct pt_regs));

	// 删除断点，即恢复程序上下文信息
	putdata(pid, hook_point->back_addr, hook_point->back_point_code.bytes, 4);
	// set_registers(pid, &(hook_point->back_regs));

	// 并把函数入口处设成断点
	set_breakpoint(pid, hook_point->entry_addr, &(hook_point->entry_point_code));

	return 0;
}

// 处理函数断点，可以一直hook住这处函数
int handle_breakpoint(pid_t pid, struct pt_regs *regs, struct Hook_point_info *hook_point)
{
	// struct pt_regs regs;
	// get_registers(pid, &regs);

	// 如果断点是函数入口
	if (regs->ARM_pc == hook_point->entry_addr)
	{
		// 保存函数相关信息
		memcpy(&(hook_point->entry_regs), &regs, sizeof(struct pt_regs));
		hook_point->back_addr = regs->ARM_lr;

		// 移除断点，即恢复程序上下文信息
		putdata(pid, hook_point->entry_addr, hook_point->entry_point_code.bytes, 4);
		// set_registers(pid, &(hook_point->entry_regs));

		//并把函数返回地址设置成断点，便于获取函数返回值
		set_breakpoint(pid, hook_point->back_addr, &(hook_point->back_point_code));

		return 0;
	}

	// 如果断点是函数执行完的返回地址
	if (regs->ARM_pc == hook_point->back_addr)
	{
		// 保存函数相关信息
		memcpy(&(hook_point->back_regs), &regs, sizeof(struct pt_regs));

		// 删除断点，即恢复程序上下文信息
		putdata(pid, hook_point->back_addr, hook_point->back_point_code.bytes, 4);
		// set_registers(pid, &(hook_point->back_regs));

		// 并把函数入口处设成断点
		set_breakpoint(pid, hook_point->entry_addr, &(hook_point->entry_point_code));

		return 0;
	}

	return -1;
}

// 移除指定地址处的断点
int remove_breakpoint(pid_t pid, unsigned long addr, union OneInstruction *oneInstruction)
{
	// 删除断点，即恢复程序上下文信息
	putdata(pid, addr, oneInstruction->bytes, 4);

	return 0;
}

// 等待程序运行到 指定地址的断点处,并返回断点地址
long long wait_breakpoint(pid_t pid, struct pt_regs *regs)
{
	int status;
	waitpid(pid, &status, WUNTRACED);
	// printf("status:%d\n", status);
	if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGILL)
	{
		// save context 保存断点处寄存器信息
		get_registers(pid, regs);

		return 0;
	}

	return -1;
}

// 获取到函数参数
int get_func_params(pid_t pid, struct pt_regs regs, long long *params, int num_params)
{
	// long long params[num_params];
	int i;
	for (i = 0; i < num_params && i < 8; i++)
	{
		params[i] = regs.uregs[i];
	}
	if (i < num_params)
	{
		long long current_sp = regs.ARM_sp;
		// printf("current_sp = 0x%llx\n", current_sp);
		union
		{
			long long val[0x100];
			uint8_t bytes[0x100 * sizeof(long long)];
		} data;

		getdata(pid, current_sp, data.bytes, ((num_params - 8) * sizeof(long long)));
		for (int j = 0; j < (num_params - 8); j++)
		{
			params[i + j] = data.val[j];
		}
	}

	for (int k = 0; k < num_params; k++)
	{
		printf("param[%d] = 0x%llx\n", k, params[k]);
	}

	return 0;
}

// 重新设置函数参数
int set_func_params(pid_t pid, struct pt_regs *regs, long long *params, int num_params)
{
	int i = 0;
	// aarch64处理器，函数传递参数，将前 8 个参数放到r0-r7，剩下的参数压入栈中
	for (i = 0; i < num_params && i < 8; i++)
	{
		regs->uregs[i] = params[i];
	}
	set_registers(pid, &regs); // 将更改后的参㶼写入寄存器
	if (i < num_params)
	{
		long long current_sp = regs->ARM_sp;
		union
		{
			long long val[0x100];
			uint8_t bytes[0x100 * sizeof(long long)];
		} data;
		for (int j = 0; j < (num_params - 8); j++)
		{
			data.val[j] = params[i + j];
		}

		putdata(pid, (unsigned long)current_sp, data.bytes, (num_params - i) * sizeof(long long));
	}
	return 0;
}

void hook(pid_t pid, uint64_t func_addr, long long *old_params, long long *new_params, int num_params)
{
	int ret;
	ptrace_attach(pid);

	struct Hook_point_info hook_point;
	hook_point.entry_addr = func_addr;

	// 把函数入口处设成断点
	set_breakpoint(pid, hook_point.entry_addr, &(hook_point.entry_point_code));

	// 运行程序
	ptrace_cont(pid);
	// 等待断点
	struct pt_regs regs;
loop:
	wait_breakpoint(pid, &regs);
	unsigned long pc = regs.ARM_pc;

	// 如果断点是函数入口
	if (pc == hook_point.entry_addr)
	{
		// 获取函数传进来的参数
		if (old_params != NULL)
		{
			get_func_params(pid, regs, old_params, num_params);
		}
		// 重新设置函数参数
		if (new_params != NULL)
		{
			set_func_params(pid, &regs, new_params, num_params);
		}

		onEnter(pid, &regs, &hook_point);
	}

	if (pc == hook_point.back_addr)
	{
		get_registers(pid, &regs);
		long long func_return_value = regs.ARM_r0;
		printf("func_return_value : 0x%llx\n", func_return_value);

		onLeave(pid, &regs, &hook_point);
	}


	if (signal_command == 0)
	{
		// 运行程序
		ptrace_cont(pid);
		goto loop;
	}

	// 如果断点是函数入口
	if (pc == hook_point.entry_addr)
	{
		// 删除断点，即恢复程序上下文信息
		putdata(pid, hook_point.back_addr, hook_point.back_point_code.bytes, 4);
	}
	if (pc == hook_point.back_addr)
	{
		// 移除断点，即恢复程序上下文信息
		putdata(pid, hook_point.entry_addr, hook_point.entry_point_code.bytes, 4);
	}
	// 运行程序
	ptrace_cont(pid);
	ptrace_detach(pid);
}

// void hook(pid_t pid, uint64_t func_addr, long long *old_params, long long *new_params, int num_params)
// {
// 	int ret;
// 	ptrace_attach(pid);

// loop:
// 	// 在函数起始点设置断点
// 	set_breakpoint(pid, func_addr);
// 	// 运行程序
// 	ptrace_cont(pid);
// 	// 等待并判断 程序运行到函数开始位置的断点处
// 	ret = wait_breakpoint(pid, func_addr);
// 	if (ret != 0)
// 	{
// 		puts("wait breakpoint error");
// 		exit(-1);
// 	}

// 	// 获取函数传进来的参数
// 	if (old_params != NULL)
// 	{
// 		get_func_params(pid, old_params, num_params);
// 	}

// 	// 重新设置函数参数
// 	if (new_params != NULL)
// 	{
// 		set_func_params(pid, new_params, num_params);
// 	}

// 	// 如果需要 获取函数返回值
// 	lr = entry_regs.ARM_lr;
// 	entry_regs.ARM_lr = 0;

// 	// 获取到传进函数的参数后 称除断点，让程序按原来的轨迹运行
// 	remove_breakpoint(pid, func_addr);
// 	// 运行程序
// 	ptrace_cont(pid);

// 	// 等待并判断 程序运行到函数返回位置的断点处
// 	ret = wait_func_return(pid);
// 	if (ret != 0)
// 	{
// 		puts("wait funciton return error");
// 		exit(-1);
// 	}

// 	long long func_return_value = leave_regs.ARM_r0;
// 	printf("func_return_value : 0x%llx\n", func_return_value);

// 	leave_regs.ARM_pc = lr;
// 	set_registers(pid, &leave_regs);

// 	if (signal_command == 0)
// 	{
// 		goto loop;
// 	}

// 	// 运行程序
// 	ptrace_cont(pid);
// }

void sighandler(int signum);

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
		return -1;
	}

	signal(SIGINT, sighandler);

	pid_t pid = atoi(argv[1]);
	uint64_t func_addr = strtoul(argv[2], NULL, 16); // 0x5d8a28373c;

	int num_params = 20;
	// 获取 原来的函数参数
	long long old_params[num_params];
	// 重新设置函数参数
	long long new_params[num_params];
	new_params[0] = 0x100;
	new_params[1] = 0x101;
	new_params[2] = 0x102;
	new_params[3] = 0x103;
	new_params[4] = 0x104;
	new_params[5] = 0x105;
	new_params[6] = 0x106;
	new_params[7] = 0x107;
	new_params[8] = 0x108;
	new_params[9] = 0x109;
	new_params[10] = 0x110;
	new_params[11] = 0x111;
	new_params[12] = 0x112;
	new_params[13] = 0x113;
	new_params[14] = 0x114;
	new_params[15] = 0x115;
	new_params[16] = 0x116;
	new_params[17] = 0x117;
	new_params[18] = 0x118;
	new_params[19] = 0x119;

	hook(pid, func_addr, old_params, new_params, num_params);

	return 0;
}

void sighandler(int signum)
{
	printf("捕获信号 %d, 等待子进程完成当前任务后,父进程会自动结束...\n", signum);
	signal_command = signum;
}
