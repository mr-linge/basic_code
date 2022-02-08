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
		perror("ptrace(GETREGS)");
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
		perror("ptrace(SETREGS)");
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
 *  Input:          pid表示远程进程的ID，ExecuteAddr为远程进程函数的地址
 *                  parameters为函数参数的地址，regs为远程进程call函数前的寄存器环境
 *  Return:         返回0表示call函数成功，返回-1表示失败
 * **********************************************/
int ptrace_call(pid_t pid, uint32_t ExecuteAddr, long *parameters, long num_params, struct pt_regs *regs)
{
	int i = 0;
	// ARM处理器，函数传递参数，将前四个参数放到r0-r3，剩下的参数压入栈中
	for (i = 0; i < num_params && i < 4; i++)
	{
		regs->uregs[i] = parameters[i];
	}
	if (i < num_params)
	{
		regs->ARM_sp -= (num_params - i) * sizeof(long);
		putdata(pid, (unsigned long)regs->ARM_sp, (uint8_t *)&parameters[i], (num_params - i) * sizeof(long));
		// if (ptrace_writedata()  == -1)
		// {
		//     return -1;
		// }
	}

	//修改程序计数器
	regs->ARM_pc = ExecuteAddr;

	//判断指令集
	// 与BX跳转指令类似，判断跳转的地址位[0]是否为1，如果为1，则将CPST寄存器的标志T置位，解释为Thumb代码
	if (regs->ARM_pc & 1)
	{
		/*Thumb*/
		regs->ARM_pc &= (~1u);
		regs->ARM_cpsr |= CPSR_T_MASK;
	}
	else
	{
		/* ARM*/
		regs->ARM_cpsr &= ~CPSR_T_MASK;
	}

	regs->ARM_lr = 0;

	//设置好寄存器后，开始运行进程
	set_registers(pid, regs);
	// if (set_registers(pid, regs) != -1)
	// {
	//     printf("ptrace set regs or continue error, pid:%d", pid);
	//     return -1;
	// }
	ptrace_cont(pid);

	//对于ptrace_continue运行的进程，他会在三种情况下进入暂停状态：1.下一次系统调用 2.子进程出现异常 3.子进程退出
	//参数WUNTRACED表示当进程进入暂停状态后，立即返回
	//将存放返回地址的lr寄存器设置为0，执行返回的时候就会发生错误，从子进程暂停
	int stat = 0;
	waitpid(pid, &stat, WUNTRACED);
	// 0xb7f表示子进程进入暂停状态
	printf("status = 0x%x\n", stat);
	while (stat != 0xb7f)
	{
		ptrace_cont(pid);
		waitpid(pid, &stat, WUNTRACED);
	}
	// 获取远程进程的寄存器值，方便获取返回值
	// if (ptrace_getregs(pid, regs) == -1)
	// {
	//     LOGD("After call getregs error");
	//     return -1;
	// }
	return 0;
}

// // 断点指令
// uint8_t trap = 0xcc;
// // 在 vaddr 处 设置断点，并获取这处的原来数据、方便以后还原
// int set_breakpoint(pid_t pid,size_t vaddr) {
// 	printf("+ Set breakpoint at %lx\n", vaddr);

// 	getdata(pid, vaddr, &orig, 1);

// 	putdata(pid, vaddr, &trap, 1);

// 	return 0;
// }

// // 判断是否运行到断点处，如果运行到断点处就获取寄存器信息
// int wait_breakpoint(pid_t pid) {
// 	int status;
// 	waitpid(pid, &status, WUNTRACED);
// 	//printf("status:%d\n", status);
// 	if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {
// 		printf("+ Child process is stopped, by breakpoint ...\n");
// 		return 0;
// 	}

// 	return -1;
// }

// // 恢复断点处的代码并使程序继续运行, 即还原寄存器信息和此位置原来的数据 然后调用 ptrace(PTRACE_CONT,...)
// int recovery_breakpoint(pid_t pid,struct pt_regs regs) {
// 	// 先运行到断点处
// //	wait_breakpoint();

// 	// 运行到断点时，pc 指向断点后一条指令处. 断点指令 0xcc 只有 1 byte 所以 -1 后就是原来断点处地址
// 	regs.ARM_pc -= 1;

// 	// 恢复寄存器数据
// 	set_registers(pid,&regs);

// 	// 清除原来设置的断点，也就是把之前写入 0xcc 处的原始数据再写回去
// 	putdata(pid, regs.ARM_pc, &orig, 1);

// 	ptrace_cont(pid);

// 	return 0;
// }

// // 远程调用函数
// int call_function(pid_t pid,size_t func_addr,long param[], uint8_t num_param,long long *result) {
// 	struct pt_regs regs;
// 	get_registers(pid, &regs);
// 	// 1.先把参数保存起来
// 	int tmp_num;
// 	if (num_param <= 6) {
// 		tmp_num = num_param;
// 	}else{
// 		tmp_num = 6;
// 		int len = num_param - 6;
// 		long *param_stack = (long *)calloc(len,8);
// 		int i,j = 0;
// 		for(i = (num_param - 1); i >= 6; i--,j++) {
// 			*(param_stack + j) = param[i];
// 		}
// 		for(int k = 0; k < len; k++) {
// 			printf("*(param_stack + %d) = %ld \t",k,*(param_stack + k));
// 		}
// 		puts("");
// 		// 第6个以后的参数保存到栈中
// 		push_stack(pid, &(regs.rsp), param_stack, len);
// 	}

// 	// 前6个参数保存在寄存器里
// 	switch(tmp_num) {
// 		case 6:
// 			regs.r9 = param[5];
// 		case 5:
// 			regs.r8 = param[4];
// 		case 4:
// 			regs.rcx = param[3];
// 		case 3:
// 			regs.rdx = param[2];
// 		case 2:
// 			regs.rsi = param[1];
// 		case 1:
// 			regs.rdi = param[0];
// 			break;
// 		case 0:
// 			{
// 				printf("no paramer ...\n" );
// 			}
// 	}
// 	//2.把当前指令的下一条指令入栈,函数往上一级返回的时候要用到
// 	// rip = rip -1 是为了让函数重新撞击断点，断点指令0xcc 就1 Byte,当上次撞击断点时 pc 已经指下了断点的下一条指令
// 	long rip[1] = {regs.rip - 1};
// 	push_stack(pid, &(regs.rsp),rip, 1);

// 	//3. 把pc指向目标函数首地址
// 	regs.rip = func_addr;
// 	set_registers(pid,&regs);
// 	ptrace_cont(pid);

// 	/*
// 	 * 为了获取 调用的目标函数的返回值(其实这个返回值存放在rax中)，只能让函数继续运行，直到运行结束 再次撞击原来的断点。
// 	 这时才是正解的时机获取到 目标函数的返回值 rax
// 	 * **/
// 	wait_breakpoint(pid);
// 	if(result != NULL) {
// 		struct pt_regs call_ret_regs;
// 		get_registers(pid, &call_ret_regs);
// 		*result = call_ret_regs.rax;
// 		//printf("call_function return value = %ld\n",*result);
// 	}

// 	return 0;
// }

// // 调用 mmap 建立匿名映射空间
// long long call_mmap(pid_t pid,unsigned long size) {
// 	size_t func_addr = get_vaddr(pid, "mmap", "libc-");
// 	printf("mmap:          		0x%lx\n", func_addr);

// 	uint8_t num_param = 6;
// 	long parameters[num_param];
// 	parameters[0] = 0;				// 设置为NULL表示让系统自动选择分配内存的地址
// 	parameters[1] = size;				// 映射内存的大小
// 	parameters[2] = PROT_READ | PROT_WRITE | PROT_EXEC; // 表示映射内存区域可读可写可执行
// 	parameters[3] = MAP_PRIVATE | MAP_ANONYMOUS;	// 建立匿名映射
// 	parameters[4] = 0;				// 若需要映射文件到内存中，则为文件的fd
// 	parameters[5] = 0;				// 文件映射偏移量

// 	long long return_value;
// 	call_function(pid, func_addr, parameters, num_param,&return_value);
// 	printf("%s %d return_value = 0x%llx\n",__FUNCTION__,__LINE__,return_value);

// 	return return_value;
// }

// // 调用 munmap 解除一个映射关系
// int call_munmap(pid_t pid,long *parameters, uint8_t num_param) {
// 	size_t func_addr = get_vaddr(pid, "munmap", "libc-");
// 	printf("munmap:                   0x%lx\n", func_addr);

// 	call_function(pid, func_addr, parameters, num_param,NULL);

// 	return 0;
// }

// // 往进程中注入动态库
// unsigned long inject_library(pid_t pid, char * lib_path) {
//         // 先获取动态库
// 	int fd;
//         void * start;
//         struct stat sb;
//         fd = open(lib_path, O_RDONLY); /* 打开/etc/passwd */
//         fstat(fd, &sb); /* 取得文件大小 */
//         start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
//         if(start == MAP_FAILED) /* 判断是否映射成功 */
//         {
//                 perror("mmap init fail");
//                 exit(-1);
//         }

// 	// 在子进程中分配一片空间，用来写入需要注入的动态库
// 	unsigned long module_addr = call_mmap(pid,sb.st_size);

// 	// 把动态库写入分配的空间中
// 	putdata(pid, module_addr, start, sb.st_size);

// 	munmap(start, sb.st_size); /* 解除映射 */
// 	close(fd);

// 	return module_addr;
// }
