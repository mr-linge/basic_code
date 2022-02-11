#include "ptrace_util.h"
#include "vaddr_by_symbol.h"
#include <signal.h>

unsigned long illegal_instruction_addr;
struct pt_regs backup_regs;
union
{
	uint32_t orig;
	uint8_t bytes[4];
} OriginOpcode;

// BX利用Rn寄存器中目的地址值的最后一位来判断跳转后的状态。当最后一位为0时，表示转移到ARM状态；当最后一位为1时，表示转移到Thumb状态。
int set_illegal_instruction(pid_t pid, unsigned long addr)
{
	getdata(pid, addr, OriginOpcode.bytes, 4);
	illegal_instruction_addr = addr;

	if (addr & (0x1))
	{
		printf("%s %d\n", __FUNCTION__, __LINE__);
		union
		{
			uint16_t uiThumbillegalValue;
			uint8_t bytes[2];
		} data;
		// Thumb  instruction : 0xdeXX
		data.uiThumbillegalValue = 0xde00;
		putdata(pid, addr, data.bytes, 2);
	}
	else
	{
		printf("%s %d\n", __FUNCTION__, __LINE__);
		union
		{
			uint32_t uiArmillegalValue;
			uint8_t bytes[4];
		} data;
		// Arm illegal instruction: 0xe7fXXXfX
		data.uiArmillegalValue = 0xe7f000f0;
		putdata(pid, addr, data.bytes, 4);
	}

	ptrace_cont(pid);
	int status;
	waitpid(pid, &status, WUNTRACED);
	// printf("status:%d\n", status);
	if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGILL)
	{
		get_registers(pid, &backup_regs);
		return 0;
	}

	return -1;
}

// 恢复异常指令
int recovery_illegal_instruction(pid_t pid)
{
	putdata(pid, illegal_instruction_addr, OriginOpcode.bytes, 4);
	set_registers(pid, &backup_regs);
	ptrace_cont(pid);

	return 0;
}

void call_inject_library_test(pid_t pid)
{
	char *lib_path = "./libinject.so";
	unsigned long module_addr = inject_library(pid, lib_path);
	printf("module_addr = 0x%lx\n", module_addr);
}

void call_munmmap_test(pid_t pid)
{
	unsigned long mmap_addr = 0x7a32125000;
	unsigned long size = 0x5000;
	call_munmap(pid, mmap_addr, size);
}

void call_mmap_test(pid_t pid)
{
	unsigned long size = 0x5000;
	unsigned long mmap_addr = call_mmap(pid, size);
	printf("mmap_addr = 0x%lx\n", mmap_addr);
}

void call_test(pid_t pid)
{
	unsigned long func_addr = 0x565b5b7a20;
	long num_params = 10;
	long param[num_params];
	param[0] = 0;
	param[1] = 1;
	param[2] = 2;
	param[3] = 3;
	param[4] = 4;
	param[5] = 5;
	param[6] = 6;
	param[7] = 7;
	param[8] = 8;
	param[9] = 9;
	// param[10] = 10;

	long long result;
	ptrace_call(pid, func_addr, param, num_params, &result);
	printf("result = 0x%llx\n", result);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
		return -1;
	}

	pid_t pid = atoi(argv[1]);

	ptrace_attach(pid);

	get_registers(pid, &backup_regs);

	// unsigned long target_addr = 0x5fb0b688ec;// regs.ARM_pc;
	// // set_illegal_instruction(pid, target_addr);
	// if(set_illegal_instruction(pid, target_addr) == -1) {
	// 	printf("set_illegal_instruction error\n");
	// 	return -1;
	// }

	call_inject_library_test(pid);

	// char *funcName = "mmap";
	// int bind = STB_GLOBAL;
	// int type = STT_FUNC;
	// unsigned long mmap_addr = get_vaddr_in_system_moudle(pid, funcName, bind, type);
	// printf("mmap_addr = 0x%lx\n", mmap_addr);

	// char *symbol = "func10";
	// char *module_path = "/data/local/tmp/work/tracee";
	// unsigned long func10_addr = get_vaddr(pid, symbol, module_path, bind, type);
	// printf("func10_addr = 0x%lx\n", func10_addr);

	// recovery_illegal_instruction(pid);
	set_registers(pid, &backup_regs);

	ptrace_cont(pid);
	ptrace_detach(pid);

	return 0;
}
