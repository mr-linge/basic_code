#include "ptrace_util.h"

//  附加到正在运行的进程
int ptrace_attach(pid_t pid)
{
	// printf("+ Attach process %d\n", pid);
	if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) < 0)
	{
		perror("ptrace(ATTACH)");
		return -1;
	}
	// printf("+ Waiting for process...\n");

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
	// printf("+ Continue process %d\n", pid);
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
	// printf("+ Detach process %d\n", pid);
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
	// printf("+ Getting registers\n");
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
	// printf("+ Setting registers\n");
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
	// printf("+ Peektext process %d\n", pid);
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
	// printf("+ Poketext process %d\n", pid);
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
