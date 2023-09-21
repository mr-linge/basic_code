#include "ptrace_util.h"
#include "vaddr_by_symbol.h"
#include <signal.h>

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

struct Hook_point_info
{
	char func_name[0x100]; 				   // 被hook函数名

	unsigned long entry_addr;			   // 待hook 函数在进程中的虚拟地址
	union OneInstruction entry_point_code; // 函数开始处的原来数据(保存后便于以后还原)
	struct pt_regs entry_regs;			   // 程序执行到函数开始处的寄存器信息
	int num_params;						   // 函数参数个数

	unsigned long back_addr;			  // 函数执行完，需要返回的地址.这里下断点为了取返回值
	union OneInstruction back_point_code; // 函数执行完返回处的原来数据(保存后便于以后还原)
	struct pt_regs back_regs;			  // 程序执行到函数返回地址处的寄存器信息
};

struct Hooked_func
{
    char func_name[0x100]; 				   // 被hook函数名
    char module_path[0x100];               // 被hook函数所在 模块，即动态库
	unsigned long func_vaddr;	           // 被hook函数虚拟地址
    int num_params;                        // 被hook函数 参数个数
};

// 用信号结束父进程的调试
void sighandler(int signum);

int hook(pid_t pid, struct Hooked_func *funcs, int func_count, unsigned long enter_call_back_vaddr, unsigned long leave_call_back_vaddr);