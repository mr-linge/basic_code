#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include "register.h"

#define PAGESIZE 0x1000
#define PAGE_START(addr) ((addr) & (~(PAGESIZE - 1)))

// 函数指针用于保留原来的执行流程
int (*old_c_test_func)(int i);

// 新函数
int new_c_test_func(int i)
{
    int origin_ret = old_c_test_func(i);
    printf("原来的参数:%d,返回值:%d\n", i, origin_ret);
    return i * 10;
}

// 计算模块在进程中的虚拟地址(即so加载到进程后的首地址)
unsigned long get_module_vaddr(char *module)
{
    char mapfile[0x20] = {0};
    pid_t pid = getpid();
    sprintf(mapfile, "/proc/%d/maps", pid);
    FILE *fd = fopen(mapfile, "r");
    if (!fd)
    {
        printf("open %s error!\n", mapfile);
        fclose(fd);
        return 0;
    }
    char buf[0x400] = {0};
    // 按行依次读取文件所有内容
    while (fgets(buf, 0x400, fd) != NULL)
    {
        if (strstr(buf, module) != NULL)
        {
            break;
        }
    }

    unsigned long module_vaddr = strtol(buf, NULL, 16);
    // printf("The process %d's module(%s) base is: 0x%lx\n", pid, module, module_vaddr);
    fclose(fd);

    return module_vaddr;
}

void hook(unsigned long origin_vaddr, unsigned long new_vaddr)
{
    int status;
    unsigned long page_start = PAGE_START(origin_vaddr);
    printf("page start:0x%lx\n", page_start);
    // printf("page end   : %lx\n", page_end);
    status = mprotect((void *)page_start, PAGESIZE, PROT_READ | PROT_WRITE);
    if (status != 0)
    {
        perror("mprotect err");
        return;
    }

    unsigned long result = 0;
    unsigned long jmp_vaddr = new_vaddr;
    printf("jmp_vaddr:0x%lx\n", jmp_vaddr);
    asm volatile(
        "mov x18, %[vaddr]                  \n"
        "mov %[result], x18                 \n"
        : [result] "=r"(result)
        : [vaddr] "r"(jmp_vaddr)
        : "x18");

    printf("result:0x%lx\n", result);
    unsigned char instruction[4] = {0};
    memcpy((void *)instruction, (void *)origin_vaddr, 4);
    puts("1 instruction:");
    for (int i = 0; i < 4; i++)
    {
        printf("%02x ", (unsigned int)instruction[i]);
    }
    puts("");

    // 40 02 1F D6
    // D6 1F 02 40             0xD6, 0x1F, 0x02, 0x40
    // 61 24 80 D2             mov x1, #0x123
    unsigned char jumpCode[4] = {0x61, 0x24, 0x80, 0xD2};
    memcpy((void *)origin_vaddr, jumpCode, 4);

    memcpy((void *)instruction, (void *)origin_vaddr, 4);
    puts("2 instruction:");
    for (int i = 0; i < 4; i++)
    {
        printf("%02x ", (unsigned int)instruction[i]);
    }
    puts("");

    status = mprotect((void *)page_start, PAGESIZE, PROT_READ | PROT_EXEC);
    if (status != 0)
    {
        perror("2 mprotect err");
        return;
    }
}

void sighandler(int signum)
{
    printf("捕获信号 %d,跳出...\n", signum);
    exit(1);
}

void __attribute__((constructor)) dylibInject(void)
{
    signal(SIGSEGV, sighandler);

    printf("Hello, hook was starting ...\n");
    char *module_name = "/data/local/tmp/main";
    unsigned long module_vaddr = get_module_vaddr(module_name);
    printf("%s --> %s vaddr:0x%lx\n", __FUNCTION__, module_name, module_vaddr);

    unsigned long target_func_vaddr = module_vaddr + 0x19C0;

    printf("new_c_test_func vaddr:0x%lx\n", (unsigned long)&new_c_test_func);
    hook(target_func_vaddr, (unsigned long)&new_c_test_func);
}
