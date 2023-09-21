#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include "ptrace_util.h"
#include "address.h"

int pid;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("cli paramer is not right.\n");
        return -1;
    }

    int status, ret;

    pid = atoi(argv[1]);
    // char *lib_path = argv[2];

    ptrace_attach();
    wait_child_signal(SIGSTOP);

    /*
    blr x2
    brk 	#0xf000
    40003FD6
    00003ED4
    **/
    char shellcode[] = {
        0xFF, 0x03, 0x04, 0xD1,
        0x21, 0x00, 0x00, 0x58,
        0x01, 0x00, 0x00, 0x14,
        0x2E, 0x2F, 0x61, 0x2E,
        0x73, 0x6F, 0x00, 0x00,
        0xE1, 0x03, 0x00, 0xF9,
        0xE0, 0x03, 0x00, 0x91,
        0x60, 0x00, 0x3F, 0xD6,
        0xFF, 0x03, 0x04, 0x91,
        0x00, 0x00, 0x3E, 0xD4,
        0x1F, 0x20, 0x03, 0xD5,
        0x1F, 0x20, 0x03, 0xD5};
    unsigned long shellcode_len = 12 * 4;
    // 备份的指令
    char backupcode[12 * 4] = {0};

    struct pt_regs regs = {0};
    get_registers(&regs);
    // get_registers(&regs_origin);

    // regs.ARM_pc = 0x592d622974;
    // set_registers(&regs);
    // ptrace_cont();

    unsigned long current_pc, new_pc, current_sp, data_stack_vaddr;
    current_pc = regs.ARM_pc;
    new_pc = 0x6174539974;//current_pc;
    current_sp = regs.ARM_sp;
    data_stack_vaddr = current_sp - 0x100;

    // 把数据保存在子进程的栈中
    // printf("%s:%d current_sp:0x%lx\n\n", __FILE__, __LINE__, current_sp);
    // char *lib_path = "AAAAAAAAAAAAA\n";
    // char data[0x100] = {0};
    // memcpy(data, lib_path, strlen(lib_path));
    // putdata(data_stack_vaddr, (void *)data, 0x100);
    // printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
    printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
    // 写入 shellcode
    // putdata(new_pc, shellcode, shellcode_len);
    printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
    // 获取要执行的子进程函数地址
    unsigned long c_module_vaddr = get_module_vaddr(pid, "libc.so");
    unsigned long printf_offset = get_symbol_offset("libc.so", "printf", 0);
    unsigned long printf_vaddr = c_module_vaddr + printf_offset;
    printf("%s:%d printf_vaddr:0x%lx\n\n", __FILE__, __LINE__, printf_vaddr);
    printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
    // regs.ARM_x0 = data_stack_vaddr;
    // regs.ARM_x2 = printf_vaddr;
    struct pt_regs regs_new = {0};
    regs_new.ARM_pc = new_pc;
    regs_new.ARM_x3 = (long long)printf_vaddr;
    set_registers(&regs_new);
    printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
    ptrace_cont();
    // wait_child_signal(SIGTRAP);
    wait_child_signal(SIGILL);
    printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
    // 写入 shellcode
    putdata(new_pc, backupcode, shellcode_len);
    set_registers(&regs);
    printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
    ptrace_cont();
    ptrace_detach();
    printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);

    return 0;
}
