#include "ptrace_util.h"

long orig = 0;
size_t addr_trap = 0;

int trap(int pid,size_t addr) {
	printf("Beginning analysis of pid: %d at %lx\n", pid, addr);
	/* 读取被追踪进程中地址为h.symaddr处的数据 */
	orig = ptrace(PTRACE_PEEKTEXT, pid, addr, NULL);

	/* int3指令码 0xcc */
	long trap = (orig & ~0xff) | 0xcc;

	/* 将int3指令替换掉 addr 的第一条指令从而设置断点 */
	if (ptrace(PTRACE_POKETEXT, pid, addr, trap) < 0) {
		perror("PTRACE_POKETEXT");
		return -1;
	}
	addr_trap = addr;
	return 0;
}

int call_function(int pid,size_t func_addr,long paramers[],int num_param,long *result) {
	/* 重启已终止的被追踪进程 */
	if (ptrace(PTRACE_CONT, pid, NULL, NULL) < 0) {
		perror("PTRACE_CONT");
		exit(-1);
	}

	int status;
	wait(&status);
	
	/* 由于breakpoint断点、trap终止运行 */
	// 判断进程是否被暂停,及暂停号是否为 SIGTRAP (SIGTRAP 实现相关的硬件异常。一般是调试异常)
	if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {
		struct user_regs_struct backup,regs;
		memset(&backup,'\0',sizeof(struct user_regs_struct));
		/* 获取寄存器内容 */
		if (ptrace(PTRACE_GETREGS, pid, NULL, &backup) < 0) {
			perror("PTRACE_GETREGS");
			exit(-1);
		}
		memcpy(&regs,&backup,sizeof(struct user_regs_struct));
		printf("%%rcx: %llx\n%%rdx: %llx\n%%rbx: %llx\n"
				"%%rax: %llx\n%%rdi: %llx\n%%rsi: %llx\n"
				"%%r8:  %llx\n%%r9:  %llx\n%%r10: %llx\n"
				"%%r11: %llx\n%%r12  %llx\n%%r13  %llx\n"
				"%%r14: %llx\n%%r15: %llx\n%%rsp: %llx\n"
				"%%rbp: %llx\n%%rip: %llx\n",
				backup.rcx, backup.rdx, backup.rbx,
				backup.rax, backup.rdi, backup.rsi,
				backup.r8,  backup.r9,  backup.r10,
				backup.r11, backup.r12, backup.r13,
				backup.r14, backup.r15, backup.rsp,
				backup.rbp, backup.rip);
		puts("**********************************************");	
		//long long regs_params[6] = {regs.%rdi，regs.%rsi，regs.%rdx，regs.%rcx，regs.%r8，regs.%r9};
		//for(int i = 0;i < 6 && i < num_param;i++) {
		//	regs_params[i] = paramers[i];
		//}

		regs.rdi = paramers[0];
		regs.rsi = paramers[1];

		/* 将指令指针-1，即从print_string的第一条指令开始执行 */
		//regs.rip = regs.rip - 1;
		regs.rip = func_addr;
		
		//regs.rsp = backup.rsp;
	//	union {
	//		long val;
	//		uint8_t bytes[8];
	//	} data;
	//	data.val = backup.rip;
	//	putdata(pid, regs.rsp - 8, data.bytes, 8);
	//	regs.rsp -= 8;
	//	regs.rbp = regs.rsp;

		printf("%%rcx: %llx\n%%rdx: %llx\n%%rbx: %llx\n"
				"%%rax: %llx\n%%rdi: %llx\n%%rsi: %llx\n"
				"%%r8:  %llx\n%%r9:  %llx\n%%r10: %llx\n"
				"%%r11: %llx\n%%r12  %llx\n%%r13  %llx\n"
				"%%r14: %llx\n%%r15: %llx\n%%rsp: %llx\n"
				"%%rbp: %llx\n%%rip: %llx\n",
				regs.rcx, regs.rdx, regs.rbx,
				regs.rax, regs.rdi, regs.rsi,
				regs.r8,  regs.r9,  regs.r10,
				regs.r11, regs.r12, regs.r13,
				regs.r14, regs.r15, regs.rsp,
				regs.rbp, regs.rip);
	
		/* 设置寄存器内容 */
		if (ptrace(PTRACE_SETREGS, pid, NULL, &regs) < 0) {
			printf("%d\n",__LINE__);
			perror("PTRACE_SETREGS");
			exit(-1);
		}

		/* 还原 function 中的第一条指令内容 */
		if (ptrace(PTRACE_POKETEXT, pid, addr_trap, orig) < 0) {
			perror("PTRACE_POKETEXT");
			exit(-1);
		}

		/* 重启已终止的被追踪进程 */
		if (ptrace(PTRACE_CONT, pid, NULL, NULL) < 0) {
			perror("PTRACE_CONT");
			exit(-1);
		}
	//	int status;
	//	wait(&status);

		
		/* 设置进程执行一条指令后切换到终止状态 */
		//  if (ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) < 0) {
		//          perror("PTRACE_SINGLESTEP");
		//          exit(-1);
		//  }

		//  wait(NULL);
	}

	return 0;
}
//int restart(int pid,size_t addr,struct user_regs_struct *regs) {
//	// 重启已终止的被追踪进程
//	if (ptrace(PTRACE_CONT, pid, NULL, NULL) < 0) {
//		perror("PTRACE_CONT");
//		exit(-1);
//	}
//
//	int status;
//	wait(&status);
//
//	// 由于breakpoint断点、trap终止运行
//	// 判断进程是否被暂停,及暂停号是否为 SIGTRAP (SIGTRAP 实现相关的硬件异常。一般是调试异常)
//	if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {
//		// 获取寄存器内容
//		if (ptrace(PTRACE_GETREGS, pid, NULL, regs) < 0) {
//			perror("PTRACE_GETREGS");
//			exit(-1);
//		}
//		printf("%%rcx: %llx\n%%rdx: %llx\n%%rbx: %llx\n"
//                                "%%rax: %llx\n%%rdi: %llx\n%%rsi: %llx\n"
//                                "%%r8:  %llx\n%%r9:  %llx\n%%r10: %llx\n"
//                                "%%r11: %llx\n%%r12  %llx\n%%r13  %llx\n"
//                                "%%r14: %llx\n%%r15: %llx\n%%rsp: %llx\n"
//                                "%%rbp: %llx\n%%rip: %llx\n",
//                                regs->rcx, regs->rdx, regs->rbx,
//                                regs->rax, regs->rdi, regs->rsi,
//                                regs->r8,  regs->r9,  regs->r10,
//                                regs->r11, regs->r12, regs->r13,
//                                regs->r14, regs->r15, regs->rsp, 
//                                regs->rbp, regs->rip);
//
//		// 还原 function 中的第一条指令内容
//		if (ptrace(PTRACE_POKETEXT, pid, addr, orig) < 0) {
//			perror("PTRACE_POKETEXT");
//			exit(-1);
//		}
//		
//		regs->rdi = 100;
//	        regs->rsi = 200;
//
//		// 将指令指针-1，即从print_string的第一条指令开始执行
//		regs->rip = regs->rip - 1;
//		//regs->rip = 0x401205;
//
//		// 设置寄存器内容
//		if (ptrace(PTRACE_SETREGS, pid, NULL, regs) < 0) {
//			perror("PTRACE_SETREGS");
//			exit(-1);
//		}
//
//		// 设置进程执行一条指令后切换到终止状态
//		//  if (ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) < 0) {
//		//          perror("PTRACE_SINGLESTEP");
//		//          exit(-1);
//		//  }
//
//		//  wait(NULL);
//	}
//
//	return 0;
//}

int remove_trap() {

	return 0;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
		return -1;
	}
	pid_t target_pid = atoi(argv[1]);
	int status;
	struct user_regs_struct backup,regs;

	ptrace_attach(target_pid);
	wait(&status);
	//get_registers(target_pid, &backup);
	size_t func1Addr = 0x4011d6;
	trap(target_pid,func1Addr);
	getchar();
	size_t func2Addr = 0x401205;
	const int num_param = 2;
	long paramers[2] = {100,200};
	call_function(target_pid,func2Addr,paramers,num_param,NULL);
	return 0;

	//	printf("%%rcx: %llx\n%%rdx: %llx\n%%rbx: %llx\n"
	//			"%%rax: %llx\n%%rdi: %llx\n%%rsi: %llx\n"
	//			"%%r8: %llx\n%%r9: %llx\n%%r10: %llx\n"
	//			"%%r11: %llx\n%%r12 %llx\n%%r13 %llx\n"
	//			"%%r14: %llx\n%%r15: %llx\n%%rsp: %llx\n",
	//			regs.rcx, regs.rdx, regs.rbx,
	//			regs.rax, regs.rdi, regs.rsi,
	//			regs.r8, regs.r9, regs.r10,
	//			regs.r11, regs.r12, regs.r13,
	//			regs.r14, regs.r15, regs.rsp);
	//	printf("%%rcx: %llx\n%%rdx: %llx\n%%rbx: %llx\n"
	//			"%%rax: %llx\n%%rdi: %llx\n%%rsi: %llx\n"
	//			"%%r8: %llx\n%%r9: %llx\n%%r10: %llx\n"
	//			"%%r11: %llx\n%%r12 %llx\n%%r13 %llx\n"
	//			"%%r14: %llx\n%%r15: %llx\n%%rsp: %llx\n",
	//			backup.rcx, backup.rdx, backup.rbx,
	//			backup.rax, backup.rdi, backup.rsi,
	//			backup.r8, backup.r9, backup.r10,
	//			backup.r11, backup.r12, backup.r13,
	//			backup.r14, backup.r15, backup.rsp);

	memcpy(&regs,&backup,sizeof(struct user_regs_struct));

	//regs.rip = 0x401205;
	//	regs.rdi = 100;
	//	regs.rsi = 101;
	//regs.rbp = backup.rip;
	set_registers(target_pid, &regs);

	printf("line:%d\n", __LINE__);
	getchar();
	printf("line:%d\n", __LINE__);
	ptrace_cont(target_pid);
	printf("line:%d\n", __LINE__);
	getchar();
	printf("line:%d\n", __LINE__);
	wait(&status);
	printf("status:%d\n", status);
	if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {
		printf("line:%d\n", __LINE__);
		set_registers(target_pid, &backup);
	}

	//	ptrace_detach(target_pid);

	return 0;
}

