#include <stdio.h>
#include <stdlib.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * 目标进程被 PTRACE_ATTACH 附加后, 子进程就已经暂停了
 当执行 PTRACE_CONT 后 子进程会继续执行
 除些之外 当前进程结束 或 PTRACE_DETACH(结束对目标进程的跟踪后)  子进程也会继续执行
 * */
int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s <pid to be traced>\n",argv[0]);
		exit(1);
	}
	pid_t traced_pid = atoi(argv[1]);
	ptrace(PTRACE_ATTACH, traced_pid, NULL, NULL);
	wait(NULL);

	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, traced_pid, NULL, &regs);

	long ins = ptrace(PTRACE_PEEKTEXT, traced_pid, regs.rip, NULL);
	printf("RIP: 0x%llx Instruction executed: %lx\n",regs.rip, ins);

	//	while(1) {
	//		sleep(3);
	//	}
	ptrace(PTRACE_DETACH, traced_pid, NULL, NULL);

	return 0;
}
