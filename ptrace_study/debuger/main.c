#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "console.h"

int pid;

enum debug_model
{
	ATTACH,
	SPAWN
};

struct option longopts[] = {
	{"attach", no_argument, NULL, 0x100},
	{"spawn", no_argument, NULL, 0x101},
	{"pid", required_argument, NULL, 0x102},
	{0, 0, 0, 0},
};

int main(int argc, char *argv[])
{
	if (argc < 4)
	{
		printf("please input pid...\n");
		return -1;
	}
	enum debug_model model;
	int c;
	while ((c = getopt_long(argc, argv, ":", longopts, NULL)) != -1)
	{
		switch (c)
		{
		case 0x100:
		{
			model = ATTACH;
		}
		break;
		case 0x101:
		{
			model = SPAWN;
		}
		break;
		case 0x102:
		{
			printf("optarg:%s\n", optarg);
			pid = atoi(optarg);
		}
		break;
		default:
			printf("unknow param:%x\n", c);
			exit(1);
			break;
		}
	}

	ptrace_attach(pid);
	// wait_child_signal(pid, SIGSTOP);
	// recv_args_console(pid);

	// while (1)
	// {
	// 	wait_child_signal(pid, SIGTRAP);
	// 	recv_args_console(pid);
	// }

	int status, ret;
lwait:
	ret = waitpid(pid, &status, WUNTRACED);
	printf("status:0x%x\n", status);
	if (ret < 0)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}

	if (WIFSTOPPED(status))
	{
		if (WSTOPSIG(status) == SIGSTOP)
		{
			recv_args_console(pid);
			ptrace_cont(pid);
			goto lwait;
		}
		else if (WSTOPSIG(status) == SIGTRAP)
		{
			recv_args_console(pid);
			ptrace_cont(pid);
			goto lwait;
		}
	}
	printf("status:0x%x\n", status);

	return 0;
}

/*
	int pid = atoi(argv[1]);
	unsigned long addr = strtoul(argv[2], NULL, 16);
	printf("addr:0x%lx\n", addr);
	int ret;

	ptrace_attach(pid);

	wait_child_signal(pid, SIGSTOP);
	set_breakpoint(pid, addr);

	ptrace_cont(pid);

	wait_child_signal(pid, SIGTRAP);

	// unsigned long len = 32;
	// unsigned char *test_data = (unsigned char *)malloc(len);
	// memset(test_data,'F',32);

	// putdata(pid, addr, test_data, len);

	struct pt_regs *regs = (struct pt_regs *)malloc(sizeof(struct pt_regs));
	get_registers(pid, regs);
	// 打印所有的寄存器
	for (int i = 0; i < 34; i++)
	{
		printf("x[%d] = %016llx\n", i, regs->uregs[i]);
	}

	regs->uregs[0] = 0x2000;
	set_registers(pid, regs);

	del_breakpoint(pid, addr);

	ptrace_cont(pid);
	ptrace_detach(pid);
**/
