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
    // 52 00 00 58             ldr x18, #8
    // 40 02 1F D6             br x18
    unsigned char jump_code[0x8] = {
        0x52, 0x00, 0x00, 0x58,
        0x40, 0x02, 0x1F, 0xD6};

    void *call_back = mmap(NULL, 0x20, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    // printf("call_back:%p\n", call_back);
    if (call_back == MAP_FAILED) /* 判断是否映射成功 */
    {
        fprintf(stderr, "%s:%d mmap error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }
    // 构造回调用函数的指令
    memcpy(call_back, target_vaddr, 0x10);
    memcpy((void *)(call_back + 0x10), jump_code, 8);
    reg.val = (unsigned long)target_vaddr + 0x10; // 跳回原函数要越过前4条指令,因为开头的4条指令被替换了
    // printf("reg.val:0x%lx\n", reg.val);
    memcpy((void *)(call_back + 24), reg.bytes, 8);

    *old_vaddr = call_back; // 将需要回调的函数指针,指向上面已经构造好的指令处

    void *target_vaddr_page_start = (void *)PAGE_START((unsigned long)target_vaddr);
    // printf("target_vaddr_page_startt:0x%lx\n", target_vaddr_page_start);
    status = mprotect(target_vaddr_page_start, PAGESIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
    if (status != 0)
    {
        fprintf(stderr, "%s:%d mprotect error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }

    // 目标函数首地址处构造跳转函数
    memcpy(target_vaddr, jump_code, 8);
    reg.val = (unsigned long)new_vaddr;
    memcpy((void *)(target_vaddr + 8), reg.bytes, 8);

    status = mprotect(target_vaddr_page_start, PAGESIZE, PROT_READ | PROT_EXEC);
    if (status != 0)
    {
        fprintf(stderr, "%s:%d mprotect error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }

    status = mprotect(call_back, PAGESIZE, PROT_READ | PROT_EXEC);
    if (status != 0)
    {
        fprintf(stderr, "%s:%d mprotect error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }
}
