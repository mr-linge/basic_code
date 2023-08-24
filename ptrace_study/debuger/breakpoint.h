#include "ptrace_util.h"

extern int pid;

// 断点指令 	brk    #0xf000
static unsigned char breakpoint[4] = {0x00, 0x00, 0x3E, 0xD4};
static unsigned char backup_machine_code[4] = {0};
// 断点信息
// struct BreakPoint
// {
//     unsigned long vaddr;
//     unsigned char backup_machine_code[4];
// };
// static unsigned long breakpoint_len = 0;
// static struct BreakPoint bp[0x100];

// 在指定地址处设置断点
void breakpoint_set( unsigned long vaddr);
// 移除指定地址处断点
void breakpoint_delete( unsigned long vaddr);

// 读取 rn 寄存器
void read_register(unsigned long index);
// 写入 rn 寄存器
void write_register(unsigned long index, unsigned long data);
// 读取内存
void read_memory(unsigned long vaddr, unsigned long len);
// 写入内存
void write_memory(unsigned long vaddr, unsigned long len, char *data);
// 从当前断点运行到下个断点
void next_breakpoint();
// 执行 1 条指令
void step_single();
