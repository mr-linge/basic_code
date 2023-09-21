#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include "ptrace_util.h"
#include "address.h"

#define DATA_LEN 0x100

int pid;

void test(char *lib_path)
{
    /*
    blr     x18
    brk 	#0xf000
    40023FD6
    00003ED4
    **/
    // 处理指令
    char shellcode[] = {
        0x40, 0x02, 0x3F, 0xD6,
        0x00, 0x00, 0x3E, 0xD4};
    unsigned long shellcode_len = sizeof(shellcode);
    printf("%d: shellcode_len:0x%lx\n\n", __LINE__, shellcode_len);
    char backupcode[0x1000] = {0}; // 备份指令

    // stack
    char data_custom[DATA_LEN] = {0}; // 自定义数据
    memcpy(data_custom, lib_path, strlen(lib_path));
    char backup_stack[0x1000] = {0}; // 备份栈上数据

    ptrace_attach();
    wait_child_signal(SIGSTOP);

    struct pt_regs regs, regs_new = {0};
    get_registers(&regs);
    memcpy(&regs_new, &regs, sizeof(struct pt_regs));

    unsigned long current_pc = (unsigned long)regs.ARM_pc;
    unsigned long new_pc = current_pc - 8;
    unsigned long current_sp = (unsigned long)regs.ARM_sp;
    // printf("%s: current_sp:0x%lx\n\n", __FILE__, current_sp);
    unsigned long data_vaddr = current_sp; // 栈上数据起始地址
    // printf("%s: data_vaddr:0x%lx\n\n", __FILE__, data_vaddr);

    // 备份指令
    getdata(new_pc, backupcode, shellcode_len);
    // 写入 shellcode
    putdata(new_pc, shellcode, shellcode_len);

    // 备份栈
    getdata(data_vaddr, backup_stack, DATA_LEN);
    // 将自己需要用到的数据写在 stack
    putdata(data_vaddr, data_custom, DATA_LEN);

    // 获取子进程 dlopen 的 vaddr
    char *dlopen_module = "libdl.so";
    unsigned long dlopen_module_vaddr = get_module_vaddr(pid, dlopen_module);
    unsigned long dlopen_offset = get_symbol_offset(dlopen_module, "dlopen", 0);
    // printf("%s: dlopen_offset:0x%lx\n\n", __FILE__, dlopen_offset);
    unsigned long dlopen_vaddr = dlopen_module_vaddr + dlopen_offset;
    // printf("%s: dlopen_vaddr:0x%lx\n\n", __FILE__, dlopen_vaddr);

    // unsigned long tmp = 0x7fcffb6b10;
    regs_new.ARM_x0 = (long long)data_vaddr;
    // regs_new.ARM_x0 = 0x7fde28d100;     // library path
    // printf("%s: regs.ARM_x0:0x%llx\n\n", __FILE__, regs.ARM_x0);
    regs_new.ARM_x1 = (long long)0x1;             // RTLD_LAZY
    regs_new.uregs[18] = (long long)dlopen_vaddr; // dlopen vaddr
    regs_new.ARM_pc = (long long)new_pc;
    // regs_new.ARM_sp = data_vaddr;
    set_registers(&regs_new);
    // printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
    ptrace_cont();
    wait_child_signal(SIGTRAP);
    // wait_child_signal(SIGILL);
    // printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
    // 恢复 shellcode 占用的指令
    putdata(new_pc, backupcode, shellcode_len);
    // 恢复栈占用的数据
    putdata(data_vaddr, backup_stack, DATA_LEN);
    set_registers(&regs);
    // printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
    ptrace_cont();
    ptrace_detach();
    // printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("cli paramer is not right.\n");
        return -1;
    }

    pid = atoi(argv[1]);
    char *lib_path = argv[2];

    test(lib_path);

    return 0;
}
