#include <stdio.h>
#include <sys/ptrace.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

#define LONGSIZE sizeof(long)

union Data
{
	long val;
	unsigned char bytes[LONGSIZE];
};

int getdata(pid_t pid, unsigned long addr, unsigned char *dst, unsigned long len)
{
	union Data data = {0};
	unsigned long i = 0;
	unsigned long j = len / LONGSIZE;
	unsigned char *laddr = dst;
	while (i < j)
	{
		data.val = ptrace(PTRACE_PEEKDATA, pid, addr + (i * LONGSIZE), NULL);
		if (data.val < 0)
		{
			fprintf(stderr, "%s:%d PTRACE_POKEDATA error: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(1);
		}
		memcpy(laddr, data.bytes, LONGSIZE);
		++i;
		laddr += LONGSIZE;
	}

	unsigned long remainder = len % LONGSIZE;
	if (remainder != 0)
	{
		data.val = ptrace(PTRACE_PEEKDATA, pid, addr + (i * LONGSIZE), NULL);
		if (data.val < 0)
		{
			fprintf(stderr, "%s:%d PTRACE_POKEDATA error: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(1);
		}
		memcpy(laddr, data.bytes, remainder);
	}

	return 0;
}

// 让子进程继续运行
int ptrace_cont(pid_t pid)
{
	if (ptrace(PTRACE_CONT, pid, NULL, NULL) < 0)
	{
		fprintf(stderr, "%s:%d PTRACE_CONT error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}
	return 0;
}

//  结束对目标进程的跟踪
int ptrace_detach(pid_t pid)
{
	if (ptrace(PTRACE_DETACH, pid, NULL, NULL) < 0)
	{
		fprintf(stderr, "%s:%d PTRACE_DETAC error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}
	return 0;
}

//  附加到正在运行的进程
int ptrace_attach(pid_t pid)
{
	if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) < 0)
	{
		fprintf(stderr, "%s:%d PTRACE_ATTACH error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}

	return 0;
}

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
		return -1;
	}

	pid_t pid = atoi(argv[1]);
	unsigned long addr = strtoul(argv[2], NULL, 16);
	printf("addr:0x%lx\n", addr);
	unsigned long len = atol(argv[3]);

	ptrace_attach(pid);
	int status = 0;
	int ret = waitpid(pid, &status, WUNTRACED);
	if (ret < 0)
	{
		fprintf(stderr, "%s:%d waitpid error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}
	printf("status = 0x%x\n", status);
	if (WIFSTOPPED(status))
	{
		printf("child pid stop:%d\n", WSTOPSIG(status));
	}

	unsigned char *dst = (unsigned char *)malloc(len);
	getdata(pid, addr, dst, len);

	for (unsigned long i = 0; i < len; i++)
	{
		printf("%02x ", (int)dst[i]);
		if ((i + 1) % 0x10 == 0)
		{
			puts("");
		}
	}
	puts("");

	ptrace_cont(pid);
	ptrace_detach(pid);

	return 0;
}
