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
#include <sys/uio.h> // struct iovec
#include <elf.h>	 // NT_PRSTATUS
#include <errno.h>

extern int pid;

#define REGISTERSIZE 8

// AArch64 寄存器结构体
struct pt_regs
{
	long long uregs[34];
};
#define ARM_x0 uregs[0]	 // 存储R0寄存器的值，函数调用后的返回值会存储在R0寄存器中.调用函数时保存第 1 个参数
#define ARM_x1 uregs[1]	 // 调用函数时保存第 2 个参数
#define ARM_x2 uregs[2]	 // 调用函数时保存第 3 个参数
#define ARM_x3 uregs[3]	 // 调用函数时保存第 4 个参数
#define ARM_x4 uregs[4]	 // 调用函数时保存第 5 个参数
#define ARM_x5 uregs[5]	 // 调用函数时保存第 6 个参数
#define ARM_x6 uregs[6]	 // 调用函数时保存第 7 个参数
#define ARM_x7 uregs[7]	 // 调用函数时保存第 8 个参数
#define ARM_fp uregs[29] // X29 is the frame pointer register (FP). 用来定位有效的栈帧记录。帧指针寄存器，存放当前过程调用栈帧的起始地址，可使用FP别名引用
#define ARM_lr uregs[30] // 链接寄存器，用于保存过程调用的返回地址，可使用LR别名引用
/*
X30 is the link register (LR). The branch-and-link instructions that store a return address in the link register (BL and BLR), setting the register X30 to PC+4.
Calls to subroutines, where it is necessary for the return address to be stored in the link register(X30).
*/
#define ARM_sp uregs[31]   // 栈指针寄存器，指向当前堆栈的栈顶
#define ARM_pc uregs[32]   // 存储当前的执行地址
#define ARM_cpsr uregs[33] // 存储状态寄存器的值

// 64 位数据单元
union DataUnit
{
	unsigned long val;
	unsigned char bytes[8];
};

// 附加到正在运行的进程
void ptrace_attach();
// 让子进程继续运行
void ptrace_cont();
// 单步执行一步
void ptrace_single_step();
// 结束对目标进程的跟踪
void ptrace_detach();

// 获取寄存器
void get_registers(struct pt_regs *regs);
// 写入寄存器
void set_registers(struct pt_regs *regs);

// 读取内存中的数据
void getdata(unsigned long addr, void *dst, unsigned long len);
// 往内存中写入数据
void putdata(unsigned long addr, void *src, unsigned long len);

// 等待子进程 指定信号
void wait_child_signal(int SIGNO);

// 使用ptrace远程调用函数
long long ptrace_call(unsigned long func_addr, long *parameters, unsigned long num);
