#include <stdio.h>
#include <sys/ptrace.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

#define LONGSIZE sizeof(long)

union Data
{
	long val;
	unsigned char bytes[LONGSIZE];
};

int putdata(pid_t pid, unsigned long addr, unsigned char *src, unsigned long len)
{
	int ret = 0;
	union Data data = {0};
	unsigned long i = 0;
	unsigned long j = len / LONGSIZE;
	unsigned char *laddr = src;
	while (i < j)
	{
		memcpy(data.bytes, laddr, LONGSIZE);
		ret = ptrace(PTRACE_POKEDATA, pid, addr + (i * LONGSIZE), data.val);
		if (ret < 0)
		{
			fprintf(stderr, "%s:%d PTRACE_POKEDATA error: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(1);
		}
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
		memcpy(data.bytes, laddr, remainder);
		ret = ptrace(PTRACE_POKEDATA, pid, addr + (i * LONGSIZE), data.val);
		if (ret < 0)
		{
			fprintf(stderr, "%s:%d PTRACE_POKEDATA error: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(1);
		}
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
		fprintf(stderr, "%s:%d PTRACE_DETACH error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}
	return 0;
}

//  附加到正在运行的进程
int ptrace_attach(pid_t target_pid)
{
	if (ptrace(PTRACE_ATTACH, target_pid, NULL, NULL) < 0)
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
		exit(1);
	}

	pid_t pid = atoi(argv[1]);
	unsigned long addr = strtoul(argv[2], NULL, 16);
	printf("addr:0x%lx\n", addr);
	char *src = argv[3];
	unsigned long len = strlen(src);
	printf("src:%s, size = %lu\n", src, len);

	ptrace_attach(pid);
	int status = 0;
	waitpid(pid, &status, WUNTRACED);
	// 0x137f PTRACE_ATTAC 后子进程暂停
	printf("status = 0x%x\n", status);

	putdata(pid, addr, (unsigned char *)src, len);

	ptrace_cont(pid);
	ptrace_detach(pid);

	return 0;
}
