#include <stdio.h>
#include <unistd.h>		// pid_t
#include <sys/ptrace.h> // ptrace
#include <sys/uio.h>	// struct iovec
#include <elf.h>		// NT_PRSTATUS
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

// AArch64 寄存器结构体
struct pt_regs
{
	long long uregs[34];
};

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("please input pid...\n");
		return -1;
	}
	long ret = 0;
	pid_t pid = atoi(argv[1]);

	ret = ptrace(PTRACE_ATTACH, pid, 0, 0);
	if (ret < 0)
	{
		fprintf(stderr, "%s:%d PTRACE_ATTACH error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}

	int status = 0;
	waitpid(pid, &status, WUNTRACED);
	// 0x137f PTRACE_ATTAC 后子进程暂停
	printf("status = 0x%x\n", status);

	struct pt_regs regs = {0};

	long regset = NT_PRSTATUS; // 如果addr是NT_PRSTATUS,则读取通用寄存器。如果addr是NT_foo,则读取浮点或向量寄存器（如果有的话)
	struct iovec ioVec = {0};
	ioVec.iov_base = &regs;
	ioVec.iov_len = sizeof(struct pt_regs);

	ret = ptrace(PTRACE_GETREGSET, pid, (void *)regset, &ioVec);
	if (ret < 0)
	{
		fprintf(stderr, "%s:%d PTRACE_GETREGSET error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}
	// 打印所有的寄存器
	for (int i = 0; i < 34; i++)
	{
		printf("x[%d] = %016llx\n", i, regs.uregs[i]);
	}

	getchar(); // 接受一个输入后程序继续往下走
	puts("往寄存器中写入数据");
	regs.uregs[0] = 0x0; // 把 pc 置为 0, 程序会发生错误,可以看效果
	ret = ptrace(PTRACE_SETREGSET, pid, (void *)regset, &ioVec);
	if (ret < 0)
	{
		fprintf(stderr, "%s:%d PTRACE_SETREGSET error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}

	puts("inter 子进程继续运行");
	getchar(); // 接受一个输入后程序继续往下走
	ret = ptrace(PTRACE_CONT, pid, NULL, NULL);
	if (ret < 0)
	{
		fprintf(stderr, "%s:%d PTRACE_CONT error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}

	waitpid(pid, &status, WUNTRACED);
	// 0xb7f 子进程出现异常(因为pc = 0)
	printf("status = 0x%x\n", status);

	puts("inter 当前进程取消对子进程的附加");
	getchar(); // 接受一个输入后程序继续往下走
	ret = ptrace(PTRACE_DETACH, pid, NULL, NULL);
	if (ret < 0)
	{
		fprintf(stderr, "%s:%d PTRACE_DETACH error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}

	return 0;
}
