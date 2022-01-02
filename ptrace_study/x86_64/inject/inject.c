#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <sys/user.h>
#include <sys/reg.h>

// 2. 获取寄存器和内存信息
int get_registers(pid_t target_pid, struct user_regs_struct *regs)  {
	printf ("+ Getting Registers\n");
	if ((ptrace (PTRACE_GETREGS, target_pid, NULL, regs)) < 0) {
		perror ("ptrace(GETREGS):");
		exit(-1);
	}

	printf ("+ Injecting shell code at %p\n", (void *)(*regs).rip);
//	inject_data (target, shellcode, (void*)regs.rip, SHELLCODE_SIZE);
	(*regs).rip += 2;

	return 0;
}

// 1. 附加到正在运行的进程
int attach_process(pid_t target_pid) {
	printf ("+ Tracing process %d\n", target_pid);
	if ((ptrace (PTRACE_ATTACH, target_pid, NULL, NULL)) < 0) {
		perror ("ptrace(ATTACH):");
		exit(-1);
	}
	printf ("+ Waiting for process...\n");
	wait (NULL);

	return 0;
}

int main (int argc, char *argv[]) {
	if (argc < 2) {
		fprintf (stderr, "Usage:\n\t%s pid\n", argv[0]);
		exit (1);
	}

	struct user_regs_struct regs;
	int                     syscall;
	long                    dst;
	pid_t target_pid = atoi (argv[1]);

	attach_process(target_pid);
	get_registers(target_pid,&regs);

	return 0;
}
