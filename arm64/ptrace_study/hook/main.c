#include "ptrace_util.h"

// 进程注入 test
unsigned long call_inject_library_test(pid_t pid)
{
	ptrace_attach(pid);
	struct pt_regs backup_regs;
	get_registers(pid, &backup_regs);

	char *lib_path = "./libinject.so";
	unsigned long module_addr = inject_library(pid, lib_path, backup_regs);
	printf("module_addr = 0x%lx\n", module_addr);

	set_registers(pid, &backup_regs);
	ptrace_cont(pid);
	ptrace_detach(pid);

	return module_addr;
}

// 调用系统函数 munmmap test
void call_munmmap_test(pid_t pid, unsigned long mmap_addr, unsigned long size)
{
	ptrace_attach(pid);
	struct pt_regs backup_regs;
	get_registers(pid, &backup_regs);

	call_munmap(pid, mmap_addr, size, backup_regs);

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
	unsigned long mmap_addr = call_mmap(pid, size, backup_regs);
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
	unsigned long func_addr = 0x5d0632ca70;
	int num_params = 20;
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

	long long result;
	ptrace_call(pid, func_addr, param, num_params, &result, backup_regs);
	printf("result = 0x%llx\n", result);

	set_registers(pid, &backup_regs);
	ptrace_cont(pid);
}

// 调用注入到进程中的动态库里的函数 test
void call_fuction_in_injectlib_test(pid_t pid)
{
	// ptrace_attach(pid);
	struct pt_regs backup_regs;
	get_registers(pid, &backup_regs);

	// 先把自己写的 动态库注入目标进程中
	char *lib_path = "./libinject.so";
	unsigned long inject_module_vaddr = inject_library(pid, lib_path, backup_regs);
	printf("inject_module_vaddr = 0x%lx\n", inject_module_vaddr);

	// 获取 注入的动态库中 func2函数在 目标进程中的地址
	char *symbol = "hook_func20";
	int bind = STB_GLOBAL;
	int type = STT_FUNC;
	unsigned long offset = offset_symbol(symbol, lib_path, bind, type);
	printf("inject module offset:         0x%lx\n", offset);
	unsigned long func_addr = inject_module_vaddr + offset;
	printf("%s = 0x%lx\n", symbol, func_addr);

	// 根据 目标进程的虚拟地址 远程调用 func2函数
	int num_params = 20;
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
	long long result;
	ptrace_call(pid, func_addr, param, num_params, &result, backup_regs);
	printf("result = 0x%llx\n", result);

	// set_registers(pid, &backup_regs);
	// ptrace_cont(pid);
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

	ptrace_cont(pid);
}

// 根据变量名 寻找到变量地址并修改变量的值
void modify_var(pid_t pid)
{
	// ptrace_attach(pid);
	// struct pt_regs backup_regs;
	// get_registers(pid, &backup_regs);

	char *symbol = "global_var";
	char *module_path = "/data/local/tmp/work/tracee";
	int bind = STB_GLOBAL;
	int type = STT_OBJECT;
	unsigned long symbol_vaddr = get_vaddr(pid, symbol, module_path, bind, type);
	printf("%s vaddr = 0x%lx\n", symbol, symbol_vaddr);

	const unsigned long size = 8;
	union
	{
		long val;
		uint8_t bytes[size];
	} data;
	data.val = 0x6666;
	putdata(pid, symbol_vaddr, data.bytes, size);

	ptrace_cont(pid);
}

// (注：没实现成功，写入失败)
void modify_var_in_injectmodule(pid_t pid)
{
	// ptrace_attach(pid);
	struct pt_regs backup_regs;
	get_registers(pid, &backup_regs);

	// printf 在目标进程中的地址
	char *symbol_printf = "printf";
	char *system_module = "/system/lib64/libc.so"; // android 系统模块路径
	int bind = STB_GLOBAL;
	int type = STT_FUNC;
	unsigned long printf_vaddr = get_vaddr(pid, symbol_printf, system_module, bind, type);
	printf("system printf_vaddr = 0x%lx\n", printf_vaddr);

	char *lib_path = "./libinject.so";
	unsigned long inject_module_vaddr = inject_library(pid, lib_path, backup_regs);
	printf("inject_module_vaddr = 0x%lx\n", inject_module_vaddr);

	char *symbol = "test_var";
	int bind1 = STB_GLOBAL;
	int type1 = STT_OBJECT;
	unsigned long symbol_offset = offset_symbol(symbol, lib_path, bind1, type1);
	unsigned long symbol_vaddr = inject_module_vaddr + symbol_offset;
	printf("%s vaddr = 0x%lx\n", symbol, symbol_vaddr);

	const unsigned long size = 8;
	union
	{
		unsigned long val;
		uint8_t bytes[size];
	} data;
	data.val = printf_vaddr;
	putdata(pid, symbol_vaddr, data.bytes, size);

	// 调用初始化 符号的函数
	char *init_symbol = "init_symbol_vaddr";
	int bind2 = STB_GLOBAL;
	int type2 = STT_FUNC;
	unsigned long init_symbol_offset = offset_symbol(init_symbol, lib_path, bind2, type2);
	printf("init_symbol_offset:         0x%lx\n", init_symbol_offset);
	unsigned long init_symbol_vaddr = inject_module_vaddr + init_symbol_offset;
	// 根据 目标进程的虚拟地址 远程调用 func2函数
	int num_params = 1;
	long param[num_params];
	param[0] = printf_vaddr;
	long long result;
	ptrace_call(pid, init_symbol_vaddr, param, num_params, &result, backup_regs);
	printf("%s %d result = 0x%llx\n", __FUNCTION__, __LINE__, result);
}

// 注入动态库并初始化 以后会 调用的一些函数地址 (可以在注入的动态库里调用目标进程里的一些函数，比如:printf)
unsigned long init_symbol_injectmodule(pid_t pid)
{
	// unsigned long inject_module_addr = call_inject_library_test(pid);
	// ptrace_attach(pid);
	struct pt_regs backup_regs;
	get_registers(pid, &backup_regs);

	char *lib_path = "./libinject.so";
	unsigned long inject_module_vaddr = inject_library(pid, lib_path, backup_regs);
	printf("inject_module_vaddr = 0x%lx\n", inject_module_vaddr);

	// printf 在目标进程中的地址
	char *symbol_printf = "printf";
	char *system_module = "/system/lib64/libc.so"; // android 系统模块路径
	int bind = STB_GLOBAL;
	int type = STT_FUNC;
	unsigned long printf_vaddr = get_vaddr(pid, symbol_printf, system_module, bind, type);
	printf("system printf_vaddr = 0x%lx\n", printf_vaddr);

	// 在注入模块中 printf_vaddr 在目标进程中的地址
	// char *inject_symbol = "printf_vaddr";
	// int bind2 = STB_GLOBAL;
	// int type2 = STT_OBJECT;
	// unsigned long inject_symbol_offset = offset_symbol(inject_symbol, lib_path, bind2, type2);
	// unsigned long inject_symbol_vaddr = inject_module_addr + inject_symbol_offset;
	// printf("inject %s = 0x%lx\n", inject_symbol, inject_symbol_vaddr);

	// 把printf 在目标进程中的地址，写到注入模块的 printf_vaddr 变量中
	// const unsigned long size = 8;
	// union
	// {
	// 	unsigned long val;
	// 	uint8_t bytes[size];
	// } data;
	// data.val = printf_vaddr;
	// putdata(pid, inject_symbol_vaddr, data.bytes, size);

	// 调用初始化 符号的函数
	char *init_symbol = "init_symbol_vaddr";
	int bind2 = STB_GLOBAL;
	int type2 = STT_FUNC;
	unsigned long init_symbol_offset = offset_symbol(init_symbol, lib_path, bind2, type2);
	printf("init_symbol_offset:         0x%lx\n", init_symbol_offset);
	unsigned long init_symbol_vaddr = inject_module_vaddr + init_symbol_offset;
	// 根据 目标进程的虚拟地址 远程调用 func2函数
	int num_params = 1;
	long param[num_params];
	param[0] = printf_vaddr;
	long long result;
	ptrace_call(pid, init_symbol_vaddr, param, num_params, &result, backup_regs);
	printf("%s %d result = 0x%llx\n", __FUNCTION__, __LINE__, result);

	// set_registers(pid, &backup_regs);
	// ptrace_cont(pid);
	// ptrace_detach(pid);

	return inject_module_vaddr;
}

// (注：没实现)
void fuction_in_injectlib_call_target_pid(pid_t pid)
{
	// ptrace_attach(pid);
	struct pt_regs backup_regs;
	get_registers(pid, &backup_regs);

	// 先把自己写的 动态库注入目标进程中
	char *lib_path = "./libinject.so";
	// unsigned long inject_module_vaddr = inject_library(pid, lib_path);
	// printf("inject_module_vaddr = 0x%lx\n", inject_module_vaddr);
	unsigned long inject_module_vaddr = init_symbol_injectmodule(pid);

	// ptrace_attach(pid);
	// struct pt_regs backup_regs;
	// get_registers(pid, &backup_regs);

	// 获取 注入的动态库中 func2函数在 目标进程中的地址
	char *symbol = "hook_func20";
	int bind3 = STB_GLOBAL;
	int type3 = STT_FUNC;
	unsigned long offset = offset_symbol(symbol, lib_path, bind3, type3);
	printf("%s:         0x%lx\n", symbol, offset);
	unsigned long func_addr = inject_module_vaddr + offset;

	// 根据 目标进程的虚拟地址 远程调用 func2函数
	int num_params = 20;
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
	long long result;
	ptrace_call(pid, func_addr, param, num_params, &result, backup_regs);
	printf("%s %d result = 0x%llx\n", __FUNCTION__, __LINE__, result);

	// set_registers(pid, &backup_regs);
	// ptrace_cont(pid);
	// ptrace_detach(pid);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
		return -1;
	}

	pid_t pid = atoi(argv[1]);

	hook_test(pid);

	// ptrace_attach(pid);
	// struct pt_regs backup_regs;
	// get_registers(pid, &backup_regs);

	// // call_fuction_test(pid);
	// call_fuction_in_injectlib_test(pid);

	// // modify_var(pid);

	// // fuction_in_injectlib_call_target_pid(pid);
	// // modify_var_in_injectmodule(pid);
	// // init_symbol_injectmodule(pid);

	// set_registers(pid, &backup_regs);
	// ptrace_cont(pid);
	// ptrace_detach(pid);

	return 0;
}
