#include "ptrace_util.h"

//  附加到正在运行的进程
int ptrace_attach(pid_t pid)
{
	printf("+ Attach process %d\n", pid);
	if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) < 0)
	{
		perror("ptrace(ATTACH)");
		return -1;
	}
	printf("+ Waiting for process...\n");

	int status;
	waitpid(pid, &status, WUNTRACED);
	// printf("status:%d\n", status);
	if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGCONT)
	{
		printf("+ Child process is stopped, by ptrace(PTRACE_ATTACH,...)\n");
		return 0;
	}

	return -1;
}

// 让子进程继续运行
int ptrace_cont(pid_t pid)
{
	printf("+ Continue process %d\n", pid);
	if (ptrace(PTRACE_CONT, pid, NULL, NULL) < 0)
	{
		perror("ptrace(PTRACE_CONT)");
		return -1;
	}
	return 0;
}

//  结束对目标进程的跟踪
int ptrace_detach(pid_t pid)
{
	printf("+ Detach process %d\n", pid);
	if (ptrace(PTRACE_DETACH, pid, NULL, NULL) < 0)
	{
		perror("ptrace(DETACH)");
		return -1;
	}
	return 0;
}

// 获取寄存器
int get_registers(pid_t pid, struct pt_regs *regs)
{
	printf("+ Getting registers\n");
	int regset = NT_PRSTATUS; //如果addr是NT_PRSTATUS，则读取通用寄存器。如果addr是NT_foo，则读取浮点或向量寄存器（如果有的话)
	struct iovec ioVec;
	ioVec.iov_base = regs;
	ioVec.iov_len = sizeof(*regs);
	if (ptrace(PTRACE_GETREGSET, pid, regset, &ioVec) < 0)
	{
		perror("ptrace(GETREGSET)");
		return -1;
	}
	return 0;
}

// 写入寄存器
int set_registers(pid_t pid, struct pt_regs *regs)
{
	printf("+ Setting registers\n");
	int regset = NT_PRSTATUS; //如果addr是NT_PRSTATUS，则读取通用寄存器。如果addr是NT_foo，则读取浮点或向量寄存器（如果有的话)
	struct iovec ioVec;
	ioVec.iov_base = regs;
	ioVec.iov_len = sizeof(*regs);
	if (ptrace(PTRACE_SETREGSET, pid, regset, &ioVec) < 0)
	{
		perror("ptrace(SETREGSET)");
		return -1;
	}
	// printf("+ Run it!\n");
	return 0;
}

// 打印寄存器信息
int show_registers(struct pt_regs regs)
{
	for (int i = 0; i < 34; i++)
	{
		printf("uregs[%d] = %llx\t", i, regs.uregs[i]);
	}
	puts("");
	printf("ARM_r0:0x%llx, ARM_r1:0x%llx, ARM_r2:0x%llx, ARM_r3:0x%llx, ARM_fp:0x%llx, ARM_lr:0x%llx, ARM_sp:0x%llx, ARM_pc:0x%llx, ARM_cpsr:0x%llx\n",
		   regs.ARM_r0, regs.ARM_r1, regs.ARM_r2, regs.ARM_r3, regs.ARM_fp, regs.ARM_lr, regs.ARM_sp, regs.ARM_pc, regs.ARM_cpsr);

	return 0;
}

// 打印此时的寄存器信息
int show_current_registers(pid_t pid)
{
	struct pt_regs regs;
	memset(&regs, '\0', sizeof(struct pt_regs));
	get_registers(pid, &regs);
	show_registers(regs);
	return 0;
}

int getdata(pid_t pid, unsigned long addr, uint8_t *dst, unsigned long len)
{
	printf("+ Peektext process %d\n", pid);
	union
	{
		long val;
		uint8_t bytes[LONGSIZE];
	} data;
	unsigned long i = 0;
	unsigned long j = len / LONGSIZE;
	uint8_t *laddr = dst;
	while (i < j)
	{
		data.val = ptrace(PTRACE_PEEKTEXT, pid, addr + (i * LONGSIZE), NULL);
		memcpy(laddr, data.bytes, LONGSIZE);
		++i;
		laddr += LONGSIZE;
	}

	unsigned long remainder = len % LONGSIZE;
	if (remainder != 0)
	{ // save the remainder, which less than LONGSIZE
		data.val = ptrace(PTRACE_PEEKTEXT, pid, addr + (i * LONGSIZE), NULL);
		memcpy(laddr, data.bytes, remainder);
	}

	return 0;
}

int putdata(pid_t pid, unsigned long addr, uint8_t *src, unsigned long len)
{
	printf("+ Poketext process %d\n", pid);
	union
	{
		long val;
		uint8_t bytes[LONGSIZE];
	} data;
	unsigned long i = 0;
	unsigned long j = len / LONGSIZE;
	uint8_t *laddr = src;
	while (i < j)
	{
		memcpy(data.bytes, laddr, LONGSIZE);
		int ret = ptrace(PTRACE_POKETEXT, pid, addr + (i * LONGSIZE), data.val);
		if (ret < 0)
		{
			perror("ptrace(POKETEXT)");
			return -1;
		}
		++i;
		laddr += LONGSIZE;
	}

	unsigned long remainder = len % LONGSIZE;
	if (remainder != 0)
	{
		data.val = ptrace(PTRACE_PEEKTEXT, pid, addr + (i * LONGSIZE), NULL);
		memcpy(data.bytes, laddr, remainder);
		ptrace(PTRACE_POKETEXT, pid, addr + (i * LONGSIZE), data.val);
	}

	return 0;
}

// 以地址 sp 为基址，往这块内堆中 push 数据，可以操纵远程进程 stack
int push_stack(int pid, unsigned long long *sp, long *param, int len)
{
	const unsigned int MAX_STACK = 80000;
	union
	{
		long val[MAX_STACK];
		uint8_t bytes[MAX_STACK * LONGSIZE];
	} data;

	unsigned long statck_delta = len * LONGSIZE;
	*sp -= statck_delta;

	for (unsigned int i = 0; i < len; i++)
	{
		data.val[len - 1 - i] = param[i];
	}

	putdata(pid, *sp, data.bytes, statck_delta);

	return 0;
}

// 以地址 sp 为基址，往这块内堆中 pop  数据，可以操纵远程进程 stack
int pop_stack(int pid, unsigned long long *sp, long *param, int len)
{
	const unsigned int MAX_STACK = 80000;
	union
	{
		long val[MAX_STACK];
		uint8_t bytes[MAX_STACK * LONGSIZE];
	} data;

	unsigned long statck_delta = len * LONGSIZE;

	// printf("pop new *sp = 0x%lx\n", *sp);
	getdata(pid, *sp, data.bytes, statck_delta);
	for (int i = 0; i < len; i++)
	{
		param[i] = data.val[i];
	}

	*sp += statck_delta;

	return 0;
}

/*************************************************
 *  Description:    使用ptrace远程call函数
 *  Input:          pid表示远程进程的ID，func_addr为远程进程函数的地址
 *                  parameters为函数参数的地址，regs为远程进程call函数前的寄存器环境
 *  Return:         返回0表示call函数成功，返回-1表示失败
 * **********************************************/
int ptrace_call(pid_t pid, unsigned long func_addr, long *parameters, long num_params, long long *result, struct pt_regs regs)
{
	int i = 0;
	// aarch64处理器，函数传递参数，将前 8 个参数放到r0-r7，剩下的参数压入栈中
	for (i = 0; i < num_params && i < 8; i++)
	{
		regs.uregs[i] = parameters[i];
	}
	if (i < num_params)
	{
		regs.ARM_sp -= (num_params - i) * sizeof(long);
		putdata(pid, (unsigned long)regs.ARM_sp, (uint8_t *)&parameters[i], (num_params - i) * sizeof(long));
	}

	//修改程序计数器
	regs.ARM_pc = func_addr;

	// 让程序执行完后，返回到 0 地址处，此时会触发异来。触发异常后 子进程会暂停，此时可以借机获取返回值
	regs.ARM_lr = 0;

	//设置好寄存器后，开始运行进程
	set_registers(pid, &regs);
	ptrace_cont(pid);

	//对于ptrace_continue运行的进程，他会在三种情况下进入暂停状态：1.下一次系统调用 2.子进程出现异常 3.子进程退出
	//参数WUNTRACED表示当进程进入暂停状态后，立即返回
	//将存放返回地址的lr寄存器设置为0，执行返回的时候就会发生错误，从子进程暂停
	int stat = 0;
	waitpid(pid, &stat, WUNTRACED);
	// 0xb7f表示子进程进入暂停状态
	//	printf("status = 0x%x\n", stat);
	while (stat != 0xb7f)
	{
				sleep(5);
		printf("%s %s %d stat = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, stat);
		ptrace_cont(pid);
		waitpid(pid, &stat, WUNTRACED);
	}
	if (result != NULL)
	{
		// 获取远程进程的寄存器值，方便获取返回值
		struct pt_regs backup_regs;
		get_registers(pid, &backup_regs);
		*result = backup_regs.ARM_r0;
	}

	return 0;
}

// 调用 mmap 建立匿名映射空间
long long call_mmap(pid_t pid, unsigned long size, struct pt_regs regs)
{
	char *module_path = "/system/lib64/libc.so"; // android 系统模块路径
	int bind = STB_GLOBAL;
	int type = STT_FUNC;
	unsigned long func_addr = get_vaddr(pid, "mmap", module_path, bind, type); // get_vaddr_in_system_moudle(pid, "mmap");
	// printf("mmap:          		0x%lx\n", func_addr);

	uint8_t num_param = 6;
	long parameters[num_param];
	parameters[0] = 0;									// 设置为NULL表示让系统自动选择分配内存的地址
	parameters[1] = size;								// 映射内存的大小
	parameters[2] = PROT_READ | PROT_WRITE | PROT_EXEC; // 表示映射内存区域可读可写可执行
	parameters[3] = MAP_PRIVATE | MAP_ANONYMOUS;		// 建立匿名映射
	parameters[4] = 0;									// 若需要映射文件到内存中，则为文件的fd
	parameters[5] = 0;									// 文件映射偏移量

	long long return_value;
	ptrace_call(pid, func_addr, parameters, num_param, &return_value, regs);
	printf("%s %s %d return_value = 0x%llx\n", __FILE__, __FUNCTION__, __LINE__, return_value);

	return return_value;
}

// 调用 munmap 解除一个映射关系
int call_munmap(pid_t pid, unsigned long start, unsigned long size, struct pt_regs regs)
{
	char *module_path = "/system/lib64/libc.so"; // android 系统模块路径
	int bind = STB_GLOBAL;
	int type = STT_FUNC;
	unsigned long func_addr = get_vaddr(pid, "munmap", module_path, bind, type);
	// size_t func_addr = get_vaddr_in_system_moudle(pid, "munmap");
	// printf("munmap:                   0x%lx\n", func_addr);

	long num_param = 2;
	long parameters[num_param];
	parameters[0] = start; // 需要解除映射的内存首地址
	parameters[1] = size;  // 内存大小
	long long return_value;
	ptrace_call(pid, func_addr, parameters, num_param, &return_value, regs);

	return (int)return_value;
}

// 往进程中注入动态库
unsigned long inject_library(pid_t pid, char *lib_path, struct pt_regs regs)
{
	// 先获取动态库
	int fd;
	void *start;
	struct stat sb;
	fd = open(lib_path, O_RDONLY); /* 打开/etc/passwd */
	fstat(fd, &sb);				   /* 取得文件大小 */
	start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (start == MAP_FAILED) /* 判断是否映射成功 */
	{
		perror("mmap init fail");
		exit(-1);
	}
	// 在子进程中分配一片空间，用来写入需要注入的动态库
	unsigned long module_addr = call_mmap(pid, sb.st_size, regs);
	// 把动态库写入分配的空间中
	putdata(pid, module_addr, start, sb.st_size);
	munmap(start, sb.st_size); /* 解除映射(当前进程中的这片空间，不是注入目标进程中的那个) */
	close(fd);

	return module_addr;
}

union
{
	uint32_t orig;
	uint8_t bytes[4];
} OriginOpcode;
// aarch64 往目标地址写入异常指令
int set_illegal_instruction(pid_t pid, unsigned long addr, struct pt_regs *backup_regs)
{
	// memset(OriginOpcode.bytes,0,sizeof(OriginOpcode.orig));
	getdata(pid, addr, OriginOpcode.bytes, 4);
	union
	{
		uint32_t uiArmillegalValue;
		uint8_t bytes[4];
	} data;
	// aarch64 illegal instruction: 0xe7fXXXfX
	data.uiArmillegalValue = 0xe7f000f0;
	putdata(pid, addr, data.bytes, 4);
	ptrace_cont(pid);
	
	return 0;
}
// 恢复异常指令
int recovery_illegal_instruction(pid_t pid, unsigned long addr, struct pt_regs backup_regs)
{
	putdata(pid, addr, OriginOpcode.bytes, 4);
	set_registers(pid, &backup_regs);

	return 0;
}
// replace function 是把目标进程中的函数替换成自己的函数
int replace_function(pid_t pid, char *target_func_name, char *module_path, char *my_func_name, char *my_lib_path, long *parameters, long num_params)
{
	// 1. 根据名字获取目标进程中 函数的地址
	int bind = STB_GLOBAL;
	int type = STT_FUNC;
	unsigned long target_func_addr = (unsigned long)get_vaddr(pid, target_func_name, module_path, bind, type);
	printf("%s vaddr = 0x%lx\n", target_func_name, target_func_addr);
	struct pt_regs illegal_regs;
	/*
		在目标地址处写入异常指令，当程序运行到目标地址时会返回 SIGILL 信号，这时说明程序已经运行到了我们想hook的函数处,
		这时的寄存器 有我们需要的 上下文信息
		比如:
		r0 - r7 是前8个传入参数
		sp		所指的栈里 第8个后的传入参数
		lr 		是这个函数运行完需要返回的地址
	*/
	// 程序运行到 设置的异常指令处时，获取此时的 寄存器，这时寄存器 lr 的值是函数运行完后要返回的地址
	int illegal_instruction_ret = set_illegal_instruction(pid, target_func_addr, &illegal_regs);
	if (illegal_instruction_ret == -1)
	{
		printf("instruction code write error\n");
		exit(-1);
	}
	// show_registers(illegal_regs);
	// 打印被hook函数的所有参数,有时根据情况会对这些参数进行处理
	long long params[num_params];
	int i;
	for (i = 0; i < num_params && i < 8; i++)
	{
		params[i] = illegal_regs.uregs[i];
	}
	if (i == 8)
	{
		long long current_sp = illegal_regs.ARM_sp;
		// printf("current_sp = 0x%llx\n", current_sp);
		union
		{
			long long val[0x100];
			uint8_t bytes[0x100 * sizeof(long)];
		} data;

		getdata(pid, current_sp, data.bytes, ((num_params - 8) * sizeof(long)));
		for (int j = 0; j < (num_params - 8); j++)
		{
			params[i + j] = data.val[j];
		}
	}
	printf("function <%s> parameters:\n", target_func_name);
	for (int k = 0; k < num_params; k++)
	{
		printf("param[%d] = 0x%llx\n", k, params[k]);
	}

	// 2. 把自己写的 动态库注入目标进程中
	unsigned long inject_module_vaddr = inject_library(pid, my_lib_path, illegal_regs);
	printf("inject_module_vaddr = 0x%lx\n", inject_module_vaddr);

	// 3. 获取 注入的动态库中 函数在 目标进程中的地址
	int bind2 = STB_GLOBAL;
	int type2 = STT_FUNC;
	unsigned long offset = offset_symbol(my_func_name, my_lib_path, bind2, type2);
	printf("inject_module function  offset:         0x%lx\n", offset);
	unsigned long func_addr = inject_module_vaddr + offset; //  模块在目标进程中的基址 加上函数在模块内的偏移 就是函数在目标进程中的虚拟地址

	// 4. 根据 目标进程的虚拟地址 远程调用 func2函数
	struct pt_regs new_regs;
	memcpy(&new_regs, &illegal_regs, sizeof(struct pt_regs));
	new_regs.ARM_sp += ((num_params - 8) * sizeof(long));
	long long result;
	ptrace_call(pid, func_addr, parameters, num_params, &result, new_regs);
	printf("result = 0x%llx\n", result);

	// 5. 调用成功后，根据需要改变程序执行逻辑，我这里就直接让函数返回 不再让被hook函数执行了.
	struct pt_regs mycode_run_regs;
	// mycode_run_regs 是自己的函数执行完后寄存器的一些信息
	get_registers(pid, &mycode_run_regs);
	// 让 pc 直接指向 被hook函数 原来的返回地址
	mycode_run_regs.ARM_pc = illegal_regs.ARM_lr;
	recovery_illegal_instruction(pid, target_func_addr, mycode_run_regs);

	return 0;
}