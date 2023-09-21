#include "ptrace_util.h"

// 附加到正在运行的进程
void ptrace_attach()
{
	if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) < 0)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}
}

// 让子进程继续运行
void ptrace_cont()
{
	if (ptrace(PTRACE_CONT, pid, NULL, NULL) < 0)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}
}

// 单步执行一步
void ptrace_single_step()
{
	if (ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) < 0)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}
}

// 结束对目标进程的跟踪
void ptrace_detach()
{
	if (ptrace(PTRACE_DETACH, pid, NULL, NULL) < 0)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}
}

// 获取寄存器
void get_registers(struct pt_regs *regs)
{
	long regset = NT_PRSTATUS; // 如果addr是NT_PRSTATUS,则读取通用寄存器。如果addr是NT_foo,则读取浮点或向量寄存器（如果有的话)
	struct iovec ioVec = {0};
	ioVec.iov_base = regs;
	ioVec.iov_len = sizeof(struct pt_regs);
	if (ptrace(PTRACE_GETREGSET, pid, (void *)regset, &ioVec) < 0)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}
}

// 写入寄存器
void set_registers(struct pt_regs *regs)
{
	long regset = NT_PRSTATUS; // 如果addr是NT_PRSTATUS,则读取通用寄存器。如果addr是NT_foo,则读取浮点或向量寄存器（如果有的话)
	struct iovec ioVec = {0};
	ioVec.iov_base = regs;
	ioVec.iov_len = sizeof(struct pt_regs);
	if (ptrace(PTRACE_SETREGSET, pid, (void *)regset, &ioVec) < 0)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}
}

void getdata(unsigned long addr, void *dst, unsigned long len)
{
	union DataUnit data = {0};
	unsigned long i = 0;
	unsigned long j = len / REGISTERSIZE;
	unsigned char *laddr = dst;
	while (i < j)
	{
		data.val = ptrace(PTRACE_PEEKTEXT, pid, addr + (i * REGISTERSIZE), NULL);
		memcpy(laddr, data.bytes, REGISTERSIZE);
		++i;
		laddr += REGISTERSIZE;
	}

	unsigned long remainder = len % REGISTERSIZE;
	if (remainder != 0)
	{
		data.val = ptrace(PTRACE_PEEKTEXT, pid, addr + (i * REGISTERSIZE), NULL);
		memcpy(laddr, data.bytes, remainder);
	}
}

void putdata(unsigned long addr, void *src, unsigned long len)
{
	int ret = 0;
	union DataUnit data = {0};
	unsigned long i = 0;
	unsigned long j = len / REGISTERSIZE;
	unsigned char *laddr = src;
	while (i < j)
	{
		memcpy(data.bytes, laddr, REGISTERSIZE);
		ret = ptrace(PTRACE_POKETEXT, pid, addr + (i * REGISTERSIZE), data.val);
		if (ret < 0)
		{
			fprintf(stderr, "%s:%d PTRACE_POKETEXT error: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(1);
		}
		++i;
		laddr += REGISTERSIZE;
	}

	unsigned long remainder = len % REGISTERSIZE;
	if (remainder != 0)
	{
		data.val = ptrace(PTRACE_PEEKTEXT, pid, addr + (i * REGISTERSIZE), NULL);
		memcpy(data.bytes, laddr, remainder);
		ret = ptrace(PTRACE_POKETEXT, pid, addr + (i * REGISTERSIZE), data.val);
		if (ret < 0)
		{
			fprintf(stderr, "%s:%d PTRACE_POKETEXT error: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(1);
		}
	}
}

void wait_child_signal(int SIGNO)
{
	int status, ret;
	do
	{
		ret = waitpid(pid, &status, WUNTRACED);
		printf("%s:%d %s %x\n", __FILE__, __LINE__, __FUNCTION__, status);
		if (ret < 0)
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(1);
		}
	} while (!(WIFSTOPPED(status) && WSTOPSIG(status) == SIGNO));
}
