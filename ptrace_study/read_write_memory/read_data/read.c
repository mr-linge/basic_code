#include <stdio.h>
#include <sys/ptrace.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


#define LONGSIZE  sizeof(long)

void getdata(pid_t target_pid, unsigned long addr, uint8_t *dst, unsigned long len) {
	union {
		long val;
		uint8_t bytes[LONGSIZE];
	} data;
	unsigned long i = 0;
	unsigned long j = len / LONGSIZE;
	uint8_t *laddr = dst;
	while (i < j) {
		data.val = ptrace(PTRACE_PEEKDATA, target_pid, addr + (i * LONGSIZE), NULL);
		memcpy(laddr, data.bytes, LONGSIZE);
		++i;
		laddr += LONGSIZE;
	}

	unsigned long remainder = len % LONGSIZE;
	if (remainder != 0) { // save the remainder, which less than LONGSIZE
		data.val = ptrace(PTRACE_PEEKDATA, target_pid, addr + (i * LONGSIZE), NULL);
		memcpy(laddr, data.bytes, remainder);
	}
}

//  结束对目标进程的跟踪
void end_tracke_process(pid_t target_pid) {
    if ((ptrace(PTRACE_DETACH, target_pid, NULL, NULL)) < 0) {
        perror("ptrace(DETACH):");
        exit(1);
    }
}

// 目标进程继续运行
void continue_process(pid_t target_pid) {
	if ((ptrace(PTRACE_CONT, target_pid, NULL, NULL)) < 0) {
		perror("ptrace(DETACH):");
		exit(1);
	}
}

//  附加到正在运行的进程
int attach_process(pid_t target_pid) {
	printf("+ Tracing process %d\n", target_pid);
	if ((ptrace(PTRACE_ATTACH, target_pid, NULL, NULL)) < 0) {
		perror("ptrace(ATTACH):");
		exit(-1);
	}
	printf("+ Waiting for process...\n");
	wait(NULL);

	return 0;
}

int main(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
		exit(1);
	}

	pid_t target_pid = atoi(argv[1]);
	unsigned long addr = atol(argv[2]);
	printf("addr : %lx\n", addr);

	attach_process(target_pid);

	unsigned long len = 32;
	uint8_t *dst = (uint8_t *) calloc(len,1);
	getdata(target_pid, addr, dst, len);

	printf("dst:%p\nvalue:%s\n", (void *) addr, dst);

	continue_process(target_pid);
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

	end_tracke_process(target_pid);

	return 0;
}

