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

#define REGISTERSIZE 8

// AArch64 寄存器结构体
struct pt_regs
{
	long long uregs[34];
};

// 64 位数据单元
union DataUnit
{
	unsigned long val;
	unsigned char bytes[REGISTERSIZE];
};

// 附加到正在运行的进程
void ptrace_attach(pid_t pid);
// 让子进程继续运行
void ptrace_cont(pid_t pid);
// 单步执行一步
void ptrace_single_step(pid_t pid);
// 结束对目标进程的跟踪
void ptrace_detach(pid_t pid);

// 获取寄存器
void get_registers(pid_t pid, struct pt_regs *regs);
// 写入寄存器
void set_registers(pid_t pid, struct pt_regs *regs);

// 读取内存中的数据
void getdata(pid_t pid, unsigned long addr, unsigned long len, unsigned char *dst);
// 往内存中写入数据
void putdata(pid_t pid, unsigned long addr, unsigned long len, unsigned char *src);

// 等待子进程 指定信号
void wait_child_signal(pid_t pid, int SIGNO);
