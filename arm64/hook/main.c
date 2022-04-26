#include "ptrace_util.h"

union OneInstruction
{
	uint32_t number;
	uint8_t bytes[4];
};

struct Hook_point_info
{
	unsigned long entry_addr;			  // 待hook 函数在进程中的虚拟地址
	union OneInstruction entry_point_code; // 函数开始处的原来数据(保存后便于以后还原)
	struct pt_regs entry_regs;			  // 程序执行到函数开始处的寄存器信息

	unsigned long back_addr;			 // 函数执行完，需要返回的地址.这里下断点为了取返回值
	union OneInstruction back_point_code; // 函数执行完返回处的原来数据(保存后便于以后还原)
	struct pt_regs back_regs;			 // 程序执行到函数返回地址处的寄存器信息
};

// 下断点并保存此位置原来数据 (aarch64 往目标地址写入异常指令)
int set_breakpoint_and_save_info(pid_t pid, unsigned long addr, union OneInstruction *opcode)
{
	// save context 保存断点处数据
	getdata(pid, addr, (*opcode).bytes, 4);

	// 下断点
	union OneInstruction illegal_instruction;
	// aarch64 illegal instruction: 0xe7fXXXfX
	illegal_instruction.number = 0xe7f000f0;
	putdata(pid, addr, illegal_instruction.bytes, 4);

	return 0;
}

// 移除断点
int remove_breakpoint(pid_t pid, unsigned long addr, struct pt_regs *backup_regs, union OneInstruction opcode)
{
	// 删除断点，即恢复程序上下文信息
	putdata(pid, addr, opcode.bytes, 4);
	set_registers(pid, backup_regs);
}

// 等待程序运行到 指定地址的断点处,然后保存断点处寄存器信息
long long wait_breakpoint(pid_t pid, unsigned long addr, struct pt_regs *backup_regs)
{
	int status;
	waitpid(pid, &status, WUNTRACED);
	// printf("status:%d\n", status);
	if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGILL)
	{
		get_registers(pid, backup_regs);
		if (backup_regs->ARM_pc == addr)
		{
			// save context 保存断点处寄存器信息
			get_registers(pid, backup_regs);
			return 0;
		}
		else
		{
			puts("并未运行到指定断点，而且运行到别的断点处");
			return backup_regs->ARM_pc;
		}
	}
	return -1;
}

// 等待函数返回值
int wait_func_return(pid_t pid, struct pt_regs *return_regs)
{
	int status;
	waitpid(pid, &status, WUNTRACED);
	while (status != 0xb7f)
	{
		ptrace_cont(pid);
		waitpid(pid, &status, WUNTRACED);
	}
	get_registers(pid, return_regs);

	return 0;
}

// 获取到函数参数
int get_func_params(pid_t pid, struct pt_regs regs, int num_params)
{
	long long params[num_params];
	int i;
	for (i = 0; i < num_params && i < 8; i++)
	{
		params[i] = regs.uregs[i];
	}
	if (i == 8)
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
int set_func_params(pid_t pid, struct pt_regs regs)
{

	return 0;
}

void hook(pid_t pid, uint64_t func_addr)
{
	int ret;
	ptrace_attach(pid);

	// 保存被hook函数的信息
	struct Hook_point_info hook_point;

	// 在函数起始点设置断点
	hook_point.entry_addr = func_addr;
	set_breakpoint_and_save_info(pid, hook_point.entry_addr, &(hook_point.entry_point_code));
	// 运行程序
	ptrace_cont(pid);
	// 等待并判断 程序运行到函数开始位置的断点处
	ret = wait_breakpoint(pid, hook_point.entry_addr, &(hook_point.entry_regs));
	if (ret != 0)
	{
		puts("wait breakpoint error");
		exit(-1);
	}

	// 获取函数传进来的参数
	get_func_params(pid, hook_point.entry_regs, 20);

	// 如果需要 获取函数返回值
	struct pt_regs tmp_regs;
	memcpy(&tmp_regs, &hook_point.entry_regs, sizeof(struct pt_regs));
	tmp_regs.ARM_lr = 0;

	// 获取到传进函数的参数后 称除断点，让程序按原来的轨迹运行
	remove_breakpoint(pid, hook_point.entry_addr, &tmp_regs, hook_point.entry_point_code);
	// 运行程序
	ptrace_cont(pid);

	// 等待并判断 程序运行到函数返回位置的断点处
	// ret = wait_breakpoint(pid, hook_point.back_addr, &(hook_point.back_regs));
	// if (ret != 0)
	// {
	// 	puts("wait breakpoint error");
	// 	exit(-1);
	// }
	struct pt_regs return_regs;
	ret = wait_func_return(pid, &return_regs);
	if (ret != 0)
	{
		puts("wait funciton return error");
		exit(-1);
	}

	long long func_return_value = return_regs.ARM_r0;
	printf("func_return_value : 0x%llx\n", func_return_value);

	return_regs.ARM_pc = hook_point.entry_regs.ARM_lr;
	set_registers(pid, &return_regs);
	// 运行程序
	ptrace_cont(pid);
}

// void hook(pid_t pid, uint64_t func_addr)
// {
// 	ptrace_attach(pid);

// 	struct pt_regs func_ptr_regs;
// 	// 程序运行到 设置的异常指令处时，获取此时的 寄存器，这时寄存器 lr 的值是函数运行完后要返回的地址
// 	int illegal_instruction_ret = set_illegal_instruction(pid, func_addr, &func_ptr_regs);
// 	if (illegal_instruction_ret == -1)
// 	{
// 		printf("instruction code write error\n");
// 		exit(-1);
// 	}
// 	int status;
// 	waitpid(pid, &status, WUNTRACED);
// 	// printf("status:%d\n", status);
// 	if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGILL)
// 	{
// 		get_registers(pid, &func_ptr_regs);
// 	}
// 	else
// 	{
// 		puts("wait status is error");
// 		return;
// 	}

// 	// 函数调用完成 返回的地址
// 	long long func_back_addr = func_ptr_regs.ARM_lr;

// 	int num_params = 20;
// 	long long params[num_params];
// 	int i;
// 	for (i = 0; i < num_params && i < 8; i++)
// 	{
// 		params[i] = func_ptr_regs.uregs[i];
// 	}
// 	if (i == 8)
// 	{
// 		long long current_sp = func_ptr_regs.ARM_sp;
// 		// printf("current_sp = 0x%llx\n", current_sp);
// 		union
// 		{
// 			long long val[0x100];
// 			uint8_t bytes[0x100 * sizeof(long long)];
// 		} data;

// 		getdata(pid, current_sp, data.bytes, ((num_params - 8) * sizeof(long long)));
// 		for (int j = 0; j < (num_params - 8); j++)
// 		{
// 			params[i + j] = data.val[j];
// 		}
// 	}
// 	for (int k = 0; k < num_params; k++)
// 	{
// 		printf("param[%d] = 0x%llx\n", k, params[k]);
// 	}

// 	// 让 pc 直接指向 被hook函数 原来的返回地址
// 	printf("func_ptr_regs.ARM_pc : 0x%llx\n", func_ptr_regs.ARM_pc);
// 	func_ptr_regs.ARM_pc = func_addr;
// 	recovery_illegal_instruction(pid, func_addr, func_ptr_regs);

// 	struct pt_regs func_back_ptr_regs;
// 	illegal_instruction_ret = set_illegal_instruction(pid, func_back_addr, &func_back_ptr_regs);
// 	if (illegal_instruction_ret == -1)
// 	{
// 		printf("instruction code write error\n");
// 		exit(-1);
// 	}
// 	status = 0;
// 	waitpid(pid, &status, WUNTRACED);
// 	// printf("status:%d\n", status);
// 	if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGILL)
// 	{
// 		get_registers(pid, &func_back_ptr_regs);
// 	}
// 	else
// 	{
// 		puts("wait status is error");
// 		return;
// 	}

// 	long long func_return_value = func_back_ptr_regs.ARM_r0;
// 	printf("func_return_value : 0x%llx\n", func_return_value);

// 	// 恢复程序执行流
// 	func_back_ptr_regs.ARM_pc = func_back_addr;
// 	recovery_illegal_instruction(pid, func_back_addr, func_ptr_regs);
// }


// hook 也就把要执行的进程中的 函数替换成 自己注入到进程里的动态库中的函数
void hook_test(pid_t pid)
{
	ptrace_attach(pid);

	char *target_func_name = "func20";
	char *module_path = "/data/local/tmp/work/tracee";

	char *my_func_name = "hook_func20";
	char *my_lib_path = "./libinject.so";
	long num_params = 20;
	long param[num_params];
	param[0] = 0x100;
	param[1] = 0x101;
	param[2] = 0x102;
	param[3] = 0x103;
	param[4] = 0x104;
	param[5] = 0x105;
	param[6] = 0x106;
	param[7] = 0x107;
	param[8] = 0x108;
	param[9] = 0x109;
	param[10] = 0x110;
	param[11] = 0x111;
	param[12] = 0x112;
	param[13] = 0x113;
	param[14] = 0x114;
	param[15] = 0x115;
	param[16] = 0x116;
	param[17] = 0x117;
	param[18] = 0x118;
	param[19] = 0x119;

	replace_function(pid, target_func_name, module_path, my_func_name, my_lib_path, param, num_params);

	ptrace_cont(pid);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
		return -1;
	}

	pid_t pid = atoi(argv[1]);

	uint64_t func_addr = strtoul(argv[2], NULL, 16); // 0x5d8a28373c;
	hook(pid, func_addr);


	// if (argc != 2)
	// {
	// 	fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
	// 	return -1;
	// }

	// pid_t pid = atoi(argv[1]);

	// hook_test(pid);

	return 0;
}
