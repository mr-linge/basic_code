#include <stdio.h>	 // printf
#include <unistd.h>	 // pid_t
#include <sys/ptrace.h>  // ptrace

// 自义 保存寄存器的 struct, 重命名这些寄存器内容便于理解
struct pt_regs {
	long long  uregs[18];
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


int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("please input pid...\n");
		return -1;
	}

	pid_t traced_process = atoi(argv[1]);
	if(traced_process == 0) {
		printf("%s is Illegal parameter\n",argv[1]);
		return -1;
	}

	struct pt_regs regs;

	long attach_ret = ptrace(PTRACE_ATTACH, traced_process, 0, 0);
	printf("attach_ret = %ld\n", attach_ret);

	long get_regs_ret = ptrace(PTRACE_GETREGSET, traced_process, NULL, &regs);
	printf("get_regs_ret = %ld\n", get_regs_ret);
	printf ("0x%llx\n", regs.ARM_pc);

	long cont_ret = ptrace(PTRACE_CONT, traced_process, NULL, NULL);
	printf("cont_ret = %ld\n", cont_ret);

	long set_regs_ret = ptrace(PTRACE_SETREGSET, traced_process, NULL, &regs);
	printf("set_regs_ret = %ld\n", set_regs_ret);

	long cont_ret2 = ptrace(PTRACE_CONT, traced_process, NULL, NULL);
	printf("cont_ret2 = %ld\n", cont_ret2);

	long detach_ret = ptrace(PTRACE_DETACH, traced_process, NULL, NULL);
	printf("detach_ret = %ld\n", detach_ret);

	return 0;
}
