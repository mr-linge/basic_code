#include "ptrace_util.h"
// #include "vaddr_by_symbol.h"

// 进程注入 test
void call_inject_library_test(pid_t pid)
{
	ptrace_attach(pid);
	struct pt_regs backup_regs;
	get_registers(pid, &backup_regs);

	char *lib_path = "./libinject.so";
	unsigned long module_addr = inject_library(pid, lib_path);
	printf("module_addr = 0x%lx\n", module_addr);

	set_registers(pid, &backup_regs);
	ptrace_cont(pid);
}

// 调用系统函数 munmmap test
void call_munmmap_test(pid_t pid, unsigned long mmap_addr, unsigned long size)
{
	ptrace_attach(pid);
	struct pt_regs backup_regs;
	get_registers(pid, &backup_regs);

	call_munmap(pid, mmap_addr, size);

	set_registers(pid, &backup_regs);
	ptrace_cont(pid);
}

// 调用系统函数 mmap test
void call_mmap_test(pid_t pid)
{
	ptrace_attach(pid);
	struct pt_regs backup_regs;
	get_registers(pid, &backup_regs);

	unsigned long size = 0x5000;
	unsigned long mmap_addr = call_mmap(pid, size);
	printf("mmap_addr = 0x%lx\n", mmap_addr);

	set_registers(pid, &backup_regs);
	ptrace_cont(pid);
}

// 调用本地函数 test
void call_fuction_test(pid_t pid)
{
	ptrace_attach(pid);
	struct pt_regs backup_regs;
	get_registers(pid, &backup_regs);

	// 本地函数地址，可用的我其他函数直接通过函数名找到对 应的地址，这里测试这个功能就只接写地址吧
	unsigned long func_addr = 0x57a3e28a20;
	int num_params = 10;
	long param[num_params];
	param[0] = 0x101;
	param[1] = 0x102;
	param[2] = 0x103;
	param[3] = 0x104;
	param[4] = 0x105;
	param[5] = 0x106;
	param[6] = 0x107;
	param[7] = 0x108;
	param[8] = 0x109;
	param[9] = 0x110;

	long long result;
	ptrace_call(pid, func_addr, param, num_params, &result);
	printf("result = 0x%llx\n", result);

	set_registers(pid, &backup_regs);
	ptrace_cont(pid);
}

// 调用注入到进程中的动态库里的函数 test
void call_fuction_in_injectlib_test(pid_t pid)
{
	ptrace_attach(pid);
	struct pt_regs backup_regs;
	get_registers(pid, &backup_regs);

	// 先把自己写的 动态库注入目标进程中
	char *symbol = "func2";
	char *lib_path = "./libinject.so";
	unsigned long module_addr = inject_library(pid, lib_path);
	printf("module_addr = 0x%lx\n", module_addr);
	printf("%s %s %d \n", __FILE__, __FUNCTION__, __LINE__);

	// 获取 注入的动态库中 func2函数在 目标进程中的地址
	int bind = STB_GLOBAL;
	int type = STT_FUNC;
	unsigned long offset = offset_symbol(symbol, lib_path, bind, type);
	printf("offset:         0x%lx\n", offset);
	unsigned long func_addr = module_addr + offset; //  模块在目标进程中的基址 加上函数在模块内的偏移 就是函数在目标进程中的虚拟地址
	printf("%s = 0x%lx\n", symbol, func_addr);

	// 根据 目标进程的虚拟地址 远程调用 func2函数
	long num_params = 2;
	long param[num_params];
	param[0] = 10;
	param[1] = 11;
	long long result;
	ptrace_call(pid, func_addr, param, num_params, &result);
	printf("result = 0x%llx\n", result);

	set_registers(pid, &backup_regs);
	ptrace_cont(pid);
}

// hook 也就把要执行的进程中的 函数替换成 自己注入到进程里的动态库中的函数
void hook_test(pid_t pid)
{
	ptrace_attach(pid);

	char *target_func_name = "func20";
	char *module_path = "/data/local/tmp/work/tracee";

	char *my_func_name = "hook_func20";
	char *my_lib_path = "./libinject.so";
	long num_params = 20;
	long param[num_params];
	param[0] = 0x100;
	param[1] = 0x101;
	param[2] = 0x102;
	param[3] = 0x103;
	param[4] = 0x104;
	param[5] = 0x105;
	param[6] = 0x106;
	param[7] = 0x107;
	param[8] = 0x108;
	param[9] = 0x109;
	param[10] = 0x110;
	param[11] = 0x111;
	param[12] = 0x112;
	param[13] = 0x113;
	param[14] = 0x114;
	param[15] = 0x115;
	param[16] = 0x116;
	param[17] = 0x117;
	param[18] = 0x118;
	param[19] = 0x119;

	replace_function(pid, target_func_name, module_path, my_func_name, my_lib_path, param, num_params);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
		return -1;
	}

	pid_t pid = atoi(argv[1]);

	// ptrace_attach(pid);

	// struct pt_regs backup_regs;
	// get_registers(pid, &backup_regs);

	// call_fuction_test(pid);
	// call_fuction_in_injectlib_test(pid);
	hook_test(pid);

	ptrace_detach(pid);

	return 0;
}
