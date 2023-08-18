#include "hook.h"

// 计算模块虚拟地址
unsigned long get_module_vaddr(char *module)
{
    char mapfile[0x20] = {0};
    sprintf(mapfile, "/proc/%d/maps", getpid());
    FILE *fd = fopen(mapfile, "r");
    if (!fd)
    {
        fprintf(stderr, "%s:%d open error: %s\n", __FILE__, __LINE__, strerror(errno));
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
    fclose(fd);

    return module_vaddr;
}

void hook(void *target_vaddr, void *new_vaddr, void **old_vaddr)
{
    int status;
    union Register reg = {0}; // 寄存器操作

    void *old_func = mmap(NULL, 32, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    printf("old_func:%p\n", old_func);
    if (old_func == MAP_FAILED) /* 判断是否映射成功 */
    {
        fprintf(stderr, "%s:%d mmap error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }
    unsigned char back_codes[32] = {0};
    memcpy(back_codes, target_vaddr, 16);
    unsigned char jump_code[0x8] = {
        0x52, 0x00, 0x00, 0x58,
        0x40, 0x02, 0x1F, 0xD6};
    memcpy((void *)(back_codes + 16), jump_code, 8);
    reg.val = (unsigned long)target_vaddr + 16;
    // printf("reg.val:0x%lx\n", reg.val);
    memcpy((void *)(back_codes + 24), reg.bytes, 8);
    // 写到 heap
    memcpy(old_func, back_codes, 32);

    *old_vaddr = old_func; // 将需要回调的函数指针,指向上面已经构造好的指令处

    unsigned long target_vaddr_page_start = PAGE_START((unsigned long)target_vaddr);
    // printf("target_vaddr_page_startt:0x%lx\n", target_vaddr_page_start);
    status = mprotect((void *)target_vaddr_page_start, PAGESIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
    if (status != 0)
    {
        fprintf(stderr, "%s:%d mprotect error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }

    // 52 00 00 58             ldr x18, #8
    // 40 02 1F D6             br x18
    unsigned char jumpCode[0x10] = {
        0x52, 0x00, 0x00, 0x58,
        0x40, 0x02, 0x1F, 0xD6};
    reg.val = (unsigned long)new_vaddr;
    memcpy((void *)(jumpCode + 8), reg.bytes, 8);
    memcpy(target_vaddr, jumpCode, 0x10);

    status = mprotect((void *)target_vaddr_page_start, PAGESIZE, PROT_READ | PROT_EXEC);
    if (status != 0)
    {
        fprintf(stderr, "%s:%d mprotect error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }

    status = mprotect(old_func, PAGESIZE, PROT_READ | PROT_EXEC);
    if (status != 0)
    {
        fprintf(stderr, "%s:%d mprotect error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }
}
