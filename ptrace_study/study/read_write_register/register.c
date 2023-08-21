#include <stdio.h>		// printf
#include <unistd.h>		// pid_t
#include <sys/ptrace.h> // ptrace
#include <sys/uio.h>	// struct iovec
#include <elf.h>		// NT_PRSTATUS
#include <sys/wait.h>
#include <string.h>

// ARM64 寄存器结构体
struct pt_regs
{
	long long uregs[34];
};
#define ARM_r0 uregs[0]	 // 存储R0寄存器的值,函数调用后的返回值会存储在R0寄存器中.调用函数时保存第 1 个参数
#define ARM_r1 uregs[1]	 // 调用函数时保存第 2 个参数
#define ARM_r2 uregs[2]	 // 调用函数时保存第 3 个参数
#define ARM_r3 uregs[3]	 // 调用函数时保存第 4 个参数
#define ARM_r4 uregs[4]	 // 调用函数时保存第 5 个参数
#define ARM_r5 uregs[5]	 // 调用函数时保存第 6 个参数
#define ARM_r6 uregs[6]	 // 调用函数时保存第 7 个参数
#define ARM_r7 uregs[7]	 // 调用函数时保存第 8 个参数
#define ARM_fp uregs[29] // X29 is the frame pointer register (FP). 用来定位有效的栈帧记录。帧指针寄存器,存放当前过程调用栈帧的起始地址,可使用FP别名引用
#define ARM_lr uregs[30] // 链接寄存器,用于保存过程调用的返回地址,可使用LR别名引用
/*
X30 is the link register (LR). The branch-and-link instructions that store a return address in the link register (BL and BLR), setting the register X30 to PC+4.
Calls to subroutines, where it is necessary for the return address to be stored in the link register(X30).
*/
#define ARM_sp uregs[31]   // 栈指针寄存器,指向当前堆栈的栈顶
#define ARM_pc uregs[32]   // 存储当前的执行地址
#define ARM_cpsr uregs[33] // 存储状态寄存器的值

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("please input pid...\n");
		return -1;
	}

	pid_t pid = atoi(argv[1]);

	long attach_ret = ptrace(PTRACE_ATTACH, pid, 0, 0);
	perror("attach");
	printf("attach_ret = %ld\n", attach_ret);
	wait(NULL);

	struct pt_regs regs;
	memset((void *)&regs, 0, sizeof(struct pt_regs));

	int regset = NT_PRSTATUS; // 如果addr是NT_PRSTATUS,则读取通用寄存器。如果addr是NT_foo,则读取浮点或向量寄存器（如果有的话)
	struct iovec ioVec;
	ioVec.iov_base = &regs;
	ioVec.iov_len = sizeof(regs);

	long get_regs_ret = ptrace(PTRACE_GETREGSET, pid, (void *)regset, &ioVec);
	perror("PTRACE_GETREGSET");
	printf("get_regs_ret = %ld\n", get_regs_ret);
	printf("regs.ARM_pc = %llx\n", regs.ARM_pc);
	for (int i = 0; i < 34; i++)
	{
		printf("uregs[%d] = %llx\n", i, regs.uregs[i]);
	}
	// printf("ARM_r0:0x%llx, ARM_r1:0x%llx, ARM_r2:0x%llx, ARM_r3:0x%llx, ARM_r4:0x%llx, ARM_r5:0x%llx, ARM_r6:0x%llx, ARM_r7:0x%llx, ARM_r8:0x%llx, ARM_r9:0x%llx, ARM_r10:0x%llx, ARM_ip:0x%llx, ARM_sp:0x%llx, ARM_lr:0x%llx, ARM_pc:0x%llx\n",regs.ARM_r0, regs.ARM_r1, regs.ARM_r2, regs.ARM_r3, regs.ARM_r4, regs.ARM_r5, regs.ARM_r6, regs.ARM_r7, regs.ARM_r8, regs.ARM_r9, regs.ARM_r10, regs.ARM_ip, regs.ARM_sp, regs.ARM_lr, regs.ARM_pc);

	regs.ARM_pc = 0x1100;
	long set_regs_ret = ptrace(PTRACE_SETREGSET, pid, (void *)regset, &ioVec);
	perror("PTRACE_SETREGSET");
	printf("set_regs_ret = %ld\n", set_regs_ret);

	long cont_ret = ptrace(PTRACE_CONT, pid, NULL, NULL);
	perror("PTRACE_CONT");
	printf("cont_ret = %ld\n", cont_ret);

	long detach_ret = ptrace(PTRACE_DETACH, pid, NULL, NULL);
	perror("PTRACE_DETACH");
	printf("detach_ret = %ld\n", detach_ret);

	return 0;
}
