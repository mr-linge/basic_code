#include "ptrace_util.h"

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

void breakpoint_set(pid_t pid, unsigned long vaddr);

void breakpoint_delete(pid_t pid, unsigned long vaddr);
