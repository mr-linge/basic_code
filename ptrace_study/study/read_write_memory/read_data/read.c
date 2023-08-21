#include <stdio.h>
#include <sys/ptrace.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define LONGSIZE sizeof(long)

int getdata(pid_t target_pid, unsigned long addr, uint8_t *dst, unsigned long len)
{
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
		data.val = ptrace(PTRACE_PEEKDATA, target_pid, addr + (i * LONGSIZE), NULL);
		memcpy(laddr, data.bytes, LONGSIZE);
		++i;
		laddr += LONGSIZE;
	}

	unsigned long remainder = len % LONGSIZE;
	if (remainder != 0)
	{ // save the remainder, which less than LONGSIZE
		data.val = ptrace(PTRACE_PEEKDATA, target_pid, addr + (i * LONGSIZE), NULL);
		memcpy(laddr, data.bytes, remainder);
	}

	return 0;
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

//  附加到正在运行的进程
int ptrace_attach(pid_t target_pid)
{
	printf("+ Tracing process %d\n", target_pid);
	if (ptrace(PTRACE_ATTACH, target_pid, NULL, NULL) < 0)
	{
		perror("ptrace(ATTACH):");
		return -1;
	}
	printf("+ Waiting for process...\n");

	return 0;
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
		return -1;
	}

	pid_t pid = atoi(argv[1]);
	unsigned long addr = strtoul(argv[2], NULL, 16);
	printf("addr : %lx\n", addr);

	ptrace_attach(pid);
	wait(NULL);

	unsigned long len = 32;
	uint8_t *dst = (uint8_t *)calloc(len, 1);
	getdata(pid, addr, dst, len);

	printf("dst:%p\nvalue:%s\n", (void *)addr, dst);

	ptrace_cont(pid);
	//	int i = 0;
	//	while(1) {
	//		i++;
	//		long value = ptrace(PTRACE_PEEKDATA, target_pid, addr, NULL);
	//		printf("dst:%p, value = %ld\n", (void *) addr, value);
	//		sleep(3);
	//		if(i == 10) {
	//			break;
	//		}
	//	}

	ptrace_detach(pid);

	return 0;
}
