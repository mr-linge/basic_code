#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include "ptrace_util.h"
#include "address.h"

int pid;

void test2()
{
    ptrace_attach();
    wait_child_signal(SIGSTOP);
    struct pt_regs regs = {0};
    get_registers(&regs);

    char *lib_path = "./liba.so";

    char *c_module = "libc.so";
    unsigned long c_module_vaddr = get_module_vaddr(pid, c_module);
    char *dlopen_module = "libdl.so";
    unsigned long dlopen_module_vaddr = get_module_vaddr(pid, dlopen_module);

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
    // printf("%s:%d mmap_vaddr:0x%lx\n\n", __FILE__, __LINE__, mmap_vaddr);
    long long mem_start = ptrace_call(mmap_vaddr, parameters, num_param);

    // 2. 在子进程的这段内存中写入数据
    putdata((unsigned long)mem_start, lib_path, strlen(lib_path));

    // 3. 调用子进程的 dlopen 函数
    // printf("%s: dlopen_module_vaddr:0x%lx\n", __FILE__, dlopen_module_vaddr);
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
    ptrace_detach();
}

// 测试寻找符号地址
void test1()
{
    printf("==================== current pid:%d ====================\n", getpid());
    char *dlopen_module = "libdl.so";
    unsigned long dlopen_module_vaddr = get_module_vaddr(getpid(), dlopen_module);
    printf("%s: dlopen_module_vaddr:0x%lx\n", __FILE__, dlopen_module_vaddr);

    unsigned long dlopen_offset = get_symbol_offset(dlopen_module, "dlopen", 0);
    printf("%s: dlopen_offset:0x%lx\n\n", __FILE__, dlopen_offset);

    char *c_module = "libc.so";
    unsigned long std_c_module_vaddr = get_module_vaddr(getpid(), c_module);
    printf("%s: std_c_module_vaddr:0x%lx\n", __FILE__, std_c_module_vaddr);

    unsigned long mmap_symbol_offset = get_symbol_offset(c_module, "mmap", 0);
    printf("%s: mmap_symbol_offset:0x%lx\n\n", __FILE__, mmap_symbol_offset);

    char *a_module = "/data/local/tmp/liba.so";
    unsigned long a_module_vaddr = get_module_vaddr(getpid(), a_module);
    printf("a_module_vaddr:0x%lx\n", a_module_vaddr);

    unsigned long add_offset = get_symbol_offset(a_module, "add", 1);
    printf("add_offset:0x%lx\n", add_offset);

    getchar();
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("cli paramer is not right.\n");
        return -1;
    }

    pid = atoi(argv[1]);

    test2();

    return 0;
}
