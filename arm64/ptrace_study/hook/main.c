#include "ptrace_util.h"
#include "vaddr_by_symbol.h"
#include <signal.h>

// BX利用Rn寄存器中目的地址值的最后一位来判断跳转后的状态。当最后一位为0时，表示转移到ARM状态；当最后一位为1时，表示转移到Thumb状态。
void write_illegal_instruction(pid_t pid, uint64_t addr, uint32_t OriginOpcode)
{
	if (addr & (0x1))
	{
		printf("%s %d\n", __FUNCTION__, __LINE__);
		union
		{
			uint32_t uiThumbillegalValue;
			uint8_t bytes[4];
		} data;
		// Thumb  instruction : 0xdeXX
		data.uiThumbillegalValue = 0x0000de00 | (0xFFFF0000 & OriginOpcode);
		//      write_data_to_addr(addr & (~0x00000001), uiThumbillegalValue);
		putdata(pid, addr & (~0x00000001), data.bytes, 4);
	}
	else
	{
		printf("%s %d\n", __FUNCTION__, __LINE__);
		// Arm illegal instruction: 0xf7fXaXXX
		// *OriginOpcode = *(uint32_t *)addr;
		union
		{
			uint32_t uiArmillegalValue;
			uint8_t bytes[4];
		} data;
		data.uiArmillegalValue = 0xe7f000f0;
		//       write_data_to_addr(addr, uiArmillegalValue);
		putdata(pid, addr, data.bytes, 4);
	}
}

static void sig_usr(int signum)
{
	printf("go here %s\n", __FUNCTION__);
	if (signum == SIGUSR1)
	{
		printf("SIGUSR1 received\n");
	}
	else if (signum == SIGUSR2)
	{
		printf("SIGUSR2 received\n");
	}
	else
	{
		printf("signal %d received\n", signum);
	}
}

int set_signal()
{
	printf("go here %s\n", __FUNCTION__);
	struct sigaction sa_usr;
	sigemptyset(&sa_usr.sa_mask);
	sa_usr.sa_handler = sig_usr; //信号处理函数
	sa_usr.sa_flags = SA_SIGINFO;

	int ret = sigaction(SIGILL, &sa_usr, NULL);
	if (ret == -1)
	{
		printf("sigaction error\n");
		return -1;
	}

	return 0;
}

void sighandler(int signum)
{
	printf("捕获信号 %d，跳出...\n", signum);
	exit(1);
}

int ptrace_call2(pid_t pid, unsigned long addr, long *params, uint32_t num_params, struct pt_regs* regs)
{
    uint8_t i;
    for (i = 0; i < num_params && i < 4; i ++) {
        regs->uregs[i] = params[i];
    }

    //
    // push remained params onto stack
    //
    if (i < num_params) {
        regs->ARM_sp -= (num_params - i) * sizeof(long) ;
        // ptrace_writedata(pid, (void *)regs->ARM_sp, (uint8_t *)&params[i], (num_params - i) * sizeof(long));
		putdata(pid, regs->ARM_sp, (uint8_t *)&params[i], (num_params - i) * sizeof(long));
    }

    regs->ARM_pc = addr;
    if (regs->ARM_pc & 1) {
        /* thumb */
        regs->ARM_pc &= (~1u);
        regs->ARM_cpsr |= CPSR_T_MASK;
    } else {
        /* arm */
        regs->ARM_cpsr &= ~CPSR_T_MASK;
    }

    // regs->ARM_lr = 0;

    // if (ptrace_setregs() == -1
    //         || ptrace_continue(pid) == -1) {
    //     printf("error\n");
    //     return -1;
    // }
	set_registers(pid, regs);
	ptrace_cont(pid);

    // int stat = 0;
    // waitpid(pid, &stat, WUNTRACED);
	// printf("stat = 0x%x\n", stat);
	// while (stat != 0xb7f) {
	// 	ptrace_cont(pid);
    //     waitpid(pid, &stat, WUNTRACED);
    // }

    return 0;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
		return -1;
	}
	// set_signal();
	// if(signal(SIGCHLD,SIG_IGN) == SIG_ERR)
	// {
	// 	perror("signal error");
	// 	exit(EXIT_FAILURE);
	// }
	// signal(SIGILL, sighandler);

	// if(signal(SIGCHLD,SIG_IGN) == SIG_ERR)
	// {
	// 	perror("signal error");
	// 	exit(EXIT_FAILURE);
	// }

	pid_t pid = atoi(argv[1]);

	ptrace_attach(pid);

	uint64_t addr = 0x6045b2a8ec;// regs.ARM_pc;
	unsigned long len = 4;
	uint8_t OriginOpcode[len];
	getdata(pid, addr, OriginOpcode, len);
	union{
		uint32_t val;
		uint8_t bytes[4];
	}data;
	// printf("%d\n", __LINE__);
	memcpy(data.bytes,OriginOpcode,len);
	write_illegal_instruction(pid, addr, data.val);


	ptrace_cont(pid);
	int status;
	waitpid(pid, &status, WUNTRACED);
	// printf("status:%d\n", status);
	if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGILL)
	{
		printf("%s + Child process is stopped, by ill instruction ...\n", __FUNCTION__);
		// show_current_registers(pid);
		printf("%d\n",__LINE__);
		struct pt_regs regs;
		memset(&regs,'\0',sizeof(struct pt_regs));
		get_registers(pid, &regs);
		show_registers(regs);


		putdata(pid, addr, OriginOpcode, len);

		unsigned long addr = 0x6045b2a944;
		uint32_t num_params = 2;
		long param[num_params];
		param[0] = 200;
		param[1] = 400;

		regs.ARM_pc = addr;
		set_registers(pid, &regs);
		printf("%d\n",__LINE__);
		ptrace_cont(pid);
		// ptrace_call2(pid, addr, param, num_params, &regs);

		return 0;
	}

	//regs.ARM_r0 = 0x101;

	//regs.ARM_pc = 0x0;
	// regs.ARM_lr = 0x0;
	// //regs.ARM_sp -= 0x16;
	// if(set_registers(pid, &regs) == -1) {
	// 	puts("set_registers error");
	// }

	// show_current_registers(pid);
	// ptrace_cont(pid);

	// int stat = 0;
	// waitpid(pid, &stat, WUNTRACED);
	// printf("ptrace call ret status is %d\n", stat);
	// //0xb7f表示子进程进入暂停状态
	// while (stat != 0xb7f)
	// {
	// 	if (ptrace_cont(pid) == -1)
	// 	{
	// 		printf("ptrace call error");
	// 		return -1;
	// 	}
	// 	waitpid(pid, &stat, WUNTRACED);
	// }

	//	size_t func_addr = 0x63e6a7f920;
	//	uint8_t num_param = 2;
	//	long parameters[num_param];
	//	parameters[0] = 0x200;
	//	parameters[1] = 0x100;
	//	ptrace_call(pid, func_addr, parameters, num_param, &regs);
	// ptrace_cont(pid);
	// while (1) {
	// 	printf("wait minutes\n");
	// 	sleep(3);
	// }

	// ptrace_cont(pid);

	return 0;
}
