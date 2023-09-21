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
		printf("%s:%d %s status:%d\n", __FILE__, __LINE__, __FUNCTION__, status);
		if (ret < 0)
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(1);
		}
	} while (!(WIFSTOPPED(status) && WSTOPSIG(status) == SIGNO));
}

/*
	Description:    使用ptrace远程call函数
	Input:          func_addr 	子进程中函数的地址
					parameters 	函数参数的地址
					num			函数参数数量
**/
long long ptrace_call(unsigned long func_addr, long *parameters, unsigned long num)
{
	struct pt_regs regs, regs_origin, regs_finish = {0};
	get_registers(&regs); // 获取此时的寄存器
	memcpy(&regs_origin, &regs, sizeof(struct pt_regs));
	// printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
	unsigned long i = 0;
	// aarch64处理器,函数传递参数,将前 8 个参数放到r0-r7,剩下的参数压入栈中
	for (i = 0; i < num && i < 8; i++)
	{
		regs.uregs[i] = parameters[i];
	}
	if (i < num)
	{
		regs.ARM_sp -= (num - i) * sizeof(long);
		putdata((unsigned long)regs.ARM_sp, (void *)&parameters[i], (num - i) * sizeof(long));
	}
	// printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
	// 修改程序计数器
	regs.ARM_pc = func_addr;
	// printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
	// 让程序执行完后,返回到 0 地址处,此时会触发异来。触发异常后 子进程会暂停,此时可以借机获取返回值
	regs.ARM_lr = 0;

	// 设置好寄存器后,开始运行进程
	set_registers(&regs);
	ptrace_cont();
	// printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
	// 将存放返回地址的lr寄存器设置为0,执行返回的时候就会发生错误,从子进程暂停
	wait_child_signal(SIGSEGV);
	// printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
	// 获取返回值
	get_registers(&regs_finish);
	// printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
	// 恢复寄存器
	set_registers(&regs_origin);
	// printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
	return regs_finish.ARM_x0;
}
