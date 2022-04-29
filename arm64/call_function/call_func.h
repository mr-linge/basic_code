#include "ptrace_util.h"
#include "vaddr_by_symbol.h"

// 保存一条 aarch64 指令的数据结构
union OneInstruction
{
	uint32_t instruction;
	uint8_t bytes[4];
};
// 全局共享变量, 一条 aarch64 指令
union OneInstruction oneInstruction;
// aarch64 illegal instruction: 0xe7fXXXfX
const static uint32_t illegal_instruction = 0xe7f000f0; // aarch64 的异常指令 0xe7f000f0

// 远程调用函数
int ptrace_call(pid_t pid, unsigned long func_addr, long *parameters, long num_params, long long *result, struct pt_regs regs);
// 调用 mmap 建立匿名映射空间
long long call_mmap(pid_t pid, unsigned long size, struct pt_regs regs);
// 调用 munmap 解除一个映射关系
int call_munmap(pid_t pid, unsigned long start, unsigned long size, struct pt_regs regs);
// 往进程中注入动态库
unsigned long inject_library(pid_t pid, char *lib_path, struct pt_regs regs);

// replace function 是把目标进程中的函数替换成自己的函数
int replace_function(pid_t pid, char *target_func_name, char *module_path, char *my_func_name, char *my_lib_path, long *parameters, long num_params);