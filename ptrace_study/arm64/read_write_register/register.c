#include <stdio.h>	 // printf
#include <unistd.h>	 // pid_t
#include <sys/ptrace.h>  // ptrace
#include <sys/user.h>    // struct user_regs_struct

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

	struct user_regs_struct regs;

	long attach_ret = ptrace(PTRACE_ATTACH, traced_process, 0, 0);
	printf("attach_ret = %ld\n", attach_ret);
	
	long get_regs_ret = ptrace(PTRACE_GETREGSET, traced_process, NULL, &regs);
	printf("get_regs_ret = %ld\n", get_regs_ret);
	printf ("0x%llx\n", regs.pc);
	
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
