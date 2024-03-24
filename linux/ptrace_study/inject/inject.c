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

// 通过构建指令集实现 dlopen 加载动态库
void test2(char *lib_path)
{
    /*
    40023FD6        blr     x18
    00003ED4        brk 	#0xf000
    **/
    // 处理指令
    char shellcode[] = {
        0x40, 0x02, 0x3F, 0xD6,
        0x00, 0x00, 0x3E, 0xD4};
    unsigned long shellcode_len = sizeof(shellcode);
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
    unsigned long data_vaddr = current_sp; // 栈上数据起始地址

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
    printf("%s:%d dlopen_module_vaddr   :0x%lx\n", __FILE__, __LINE__, dlopen_module_vaddr);
    unsigned long dlopen_offset = get_symbol_offset(dlopen_module, "dlopen", 0);
    printf("%s:%d dlopen_offset         :0x%lx\n", __FILE__, __LINE__, dlopen_offset);
    unsigned long dlopen_vaddr = dlopen_module_vaddr + dlopen_offset;
    printf("%s:%d dlopen_vaddr          :0x%lx\n", __FILE__, __LINE__, dlopen_vaddr);

    regs_new.ARM_x0 = (long long)data_vaddr;
    regs_new.ARM_x1 = (long long)0x1;             // RTLD_LAZY
    regs_new.uregs[18] = (long long)dlopen_vaddr; // dlopen vaddr
    regs_new.ARM_pc = (long long)new_pc;
    set_registers(&regs_new);
    ptrace_cont();
    wait_child_signal(SIGTRAP);
    // 恢复 shellcode 占用的指令
    putdata(new_pc, backupcode, shellcode_len);
    // 恢复栈占用的数据
    putdata(data_vaddr, backup_stack, DATA_LEN);
    // 恢复寄存器
    set_registers(&regs);
    ptrace_cont();
    // ptrace_detach();
}

// ptrace 调用 dlopen 加载动态库
void test1(char *lib_path)
{
    ptrace_attach();
    wait_child_signal(SIGSTOP);
    struct pt_regs regs = {0};
    get_registers(&regs);

    char *c_module = "libc.so";
    unsigned long c_module_vaddr = get_module_vaddr(pid, c_module);
    char *dlopen_module = "libdl.so";
    unsigned long dlopen_module_vaddr = get_module_vaddr(pid, dlopen_module);
    // printf("%s: dlopen_module_vaddr:0x%lx\n", __FILE__, dlopen_module_vaddr);

    // 1. 先调用 mmap 开辟一段内存
    unsigned num_param = 6;
    long parameters[num_param];
    parameters[0] = 0;                                  // 设置为NULL表示让系统自动选择分配内存的地址
    parameters[1] = 0x100;                              // 映射内存的大小
    parameters[2] = PROT_READ | PROT_WRITE | PROT_EXEC; // 表示映射内存区域可读可写可执行
    parameters[3] = MAP_PRIVATE | MAP_ANONYMOUS;        // 建立匿名映射
    parameters[4] = 0;                                  // 若需要映射文件到内存中，则为文件的fd
    parameters[5] = 0;                                  // 文件映射偏移量

    unsigned long mmap_offset = get_symbol_offset(c_module, "mmap", 0);
    unsigned long mmap_vaddr = c_module_vaddr + mmap_offset;
    printf("%s:%d mmap_vaddr:0x%lx\n\n", __FILE__, __LINE__, mmap_vaddr);
    long long mem_start = ptrace_call(mmap_vaddr, parameters, num_param);

    // 2. 在子进程的这段内存中写入数据
    putdata((unsigned long)mem_start, lib_path, strlen(lib_path));

    // 3. 调用子进程的 dlopen 函数
    unsigned long dlopen_offset = get_symbol_offset(dlopen_module, "dlopen", 0);
    printf("%s: dlopen_offset:0x%lx\n\n", __FILE__, dlopen_offset);
    unsigned long dlopen_vaddr = dlopen_module_vaddr + dlopen_offset;
    printf("%s: dlopen_vaddr:0x%lx\n\n", __FILE__, dlopen_vaddr);

    unsigned long num = 0x2;
    long parameter[0x2] = {0};
    parameter[0] = (unsigned long)mem_start;
    parameter[1] = 0x2;
    // 调用子进程函数
    ptrace_call(dlopen_vaddr, parameter, num);
    // ptrace_detach();
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

    test2(lib_path);
    // test1(lib_path);

    return 0;
}
