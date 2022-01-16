#include <stdio.h>	 // printf
#include <unistd.h>	 // pid_t
#include <sys/ptrace.h>  // ptrace
#include <stdlib.h>
#include <sys/wait.h>


// 自义 保存寄存器的 struct, 重命名这些寄存器内容便于理解
struct pt_regs {
	long long uregs[18];
};
#define ARM_cpsr uregs[16]
#define ARM_pc   uregs[15]
#define ARM_lr   uregs[14]
#define ARM_sp   uregs[13]
#define ARM_ip   uregs[12]
#define ARM_fp   uregs[11]
#define ARM_r10  uregs[10]
#define ARM_r9   uregs[9]
#define ARM_r8   uregs[8]
#define ARM_r7   uregs[7]
#define ARM_r6   uregs[6]
#define ARM_r5   uregs[5]
#define ARM_r4   uregs[4]
#define ARM_r3   uregs[3]
#define ARM_r2   uregs[2]
#define ARM_r1   uregs[1]
#define ARM_r0   uregs[0]
#define ARM_ORIG_r0 uregs[17]


//  结束对目标进程的跟踪
void end_tracke_process(pid_t target_pid) {
	if ((ptrace(PTRACE_DETACH, target_pid, NULL, NULL)) != -1) {
		perror("ptrace(DETACH):");
		exit(1);
	}
}

// 让子进程继续运行
void continue_process(pid_t target_pid) {
	if ((ptrace(PTRACE_CONT, target_pid, NULL, NULL)) > 0) {
		perror("ptrace(PTRACE_CONT):");
		exit(1);
	}
}

//  恢复寄存器
void set_registers(pid_t target_pid, struct pt_regs *regs) {
	printf("+ Setting instruction pointer to %p\n", (void *) regs->ARM_pc);
	if ((ptrace(PTRACE_SETREGSET, target_pid, NULL, regs)) != -1) {
		perror("ptrace(GETREGS):");
		exit(1);
	}
	printf("+ Run it!\n");
}

// 获取寄存器
int get_registers(pid_t target_pid, struct pt_regs *regs) {
	printf("+ Getting Registers\n");
	if ((ptrace(PTRACE_GETREGSET, target_pid, NULL, regs)) != -1) {
		perror("ptrace(GETREGS):");
		exit(-1);
	}
	return 0;
}

//  附加到正在运行的进程
int attach_process(pid_t target_pid) {
	printf("+ Tracing process %d\n", target_pid);
	if ((ptrace(PTRACE_ATTACH, target_pid, NULL, NULL)) < 0) {
		perror("ptrace(ATTACH):");
		exit(-1);
	}
	printf("+ Waiting for process...\n");

	return 0;
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("please input pid...\n");
		return -1;
	}

	pid_t target_pid = atoi(argv[1]);
	if(target_pid == 0) {
		printf("%s is Illegal parameter\n",argv[1]);
		return -1;
	}

	struct pt_regs regs;

	attach_process(target_pid);
	wait(NULL);

	get_registers(target_pid, &regs);
	printf("ARM_r0:0x%llx, ARM_r1:0x%llx, ARM_r2:0x%llx, ARM_r3:0x%llx, ARM_r4:0x%llx, ARM_r5:0x%llx, ARM_r6:0x%llx, ARM_r7:0x%llx, ARM_r8:0x%llx, ARM_r9:0x%llx, ARM_r10:0x%llx, ARM_ip:0x%llx, ARM_sp:0x%llx, ARM_lr:0x%llx, ARM_pc:0x%llx\n",regs.ARM_r0, regs.ARM_r1, regs.ARM_r2, regs.ARM_r3, regs.ARM_r4, regs.ARM_r5, regs.ARM_r6, regs.ARM_r7, regs.ARM_r8, regs.ARM_r9, regs.ARM_r10, regs.ARM_ip, regs.ARM_sp, regs.ARM_lr, regs.ARM_pc);
	
	continue_process(target_pid);
	
	set_registers(target_pid, &regs);
	
	continue_process(target_pid);

	return 0;
}
