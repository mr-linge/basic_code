#include <stdio.h>
#include <sys/ptrace.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/uio.h>    // struct iovec
#include <elf.h>        // NT_PRSTATUS

#include "vaddr_by_symbol.h"


// ARM64 寄存器结构体
struct pt_regs
{
	long long uregs[34];
};
#define ARM_r0   	uregs[0]            		// 存储R0寄存器的值，函数调用后的返回值会存储在R0寄存器中.调用函数时保存第 1 个参数
#define ARM_r1   	uregs[1]			// 调用函数时保存第 2 个参数
#define ARM_r2   	uregs[2]			// 调用函数时保存第 3 个参数
#define ARM_r3   	uregs[3]			// 调用函数时保存第 4 个参数
#define ARM_fp 		uregs[29]			// X29 is the frame pointer register (FP). 用来定位有效的栈帧记录。
#define ARM_lr 		uregs[30]			// 远程进程的函数调用结束后，程序会跳转到LR寄存器存储的地址
/*
X30 is the link register (LR). The branch-and-link instructions that store a return address in the link register (BL and BLR), setting the register X30 to PC+4.
Calls to subroutines, where it is necessary for the return address to be stored in the link register(X30).
*/
#define ARM_sp 		uregs[31]			// 存储当前的栈顶地址
#define ARM_pc 		uregs[32]			// 存储当前的执行地址
#define ARM_cpsr 	uregs[33] 			// 存储状态寄存器的值


#define CPSR_T_MASK (1u << 5)
#define LONGSIZE  sizeof(long)


// 附加到正在运行的进程
int ptrace_attach(pid_t pid);
// 让子进程继续运行
int ptrace_cont(pid_t pid);
// 结束对目标进程的跟踪
int ptrace_detach(pid_t pid);


// 获取寄存器
int get_registers(pid_t pid, struct pt_regs *regs);
// 写入寄存器
int set_registers(pid_t pid, struct pt_regs *regs);
// 打印寄存器信息
int show_registers(struct pt_regs regs);
// 打印此时的寄存器信息
int show_current_registers(pid_t pid);


// 读取内存中的数据
int getdata(pid_t pid, unsigned long addr, uint8_t *dst, unsigned long len);
// 往内存中写入数据
int putdata(pid_t pid, unsigned long addr, uint8_t *src, unsigned long len);


// 以地址 sp 为基址，往这块内堆中 push 数据，可以操纵远程进程 stack
int push_stack(int pid, unsigned long long *sp, long * param,int len);
// 以地址 sp 为基址，往这块内堆中 pop  数据，可以操纵远程进程 stack
int pop_stack(int pid, unsigned long long *sp, long *param, int len);


// 远程调用函数
int ptrace_call(pid_t pid, unsigned long func_addr, long *parameters, long num_params, long long *result);
// 调用 mmap 建立匿名映射空间
long long call_mmap(pid_t pid,unsigned long size);
// 调用 munmap 解除一个映射关系
int call_munmap(pid_t pid,unsigned long start, unsigned long size);
// 往进程中注入动态库
unsigned long inject_library(pid_t pid, char * lib_path);

