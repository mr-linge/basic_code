#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/mman.h>

// 获取 addr 页的上边界和下边界地址(页的上边界)
#define PAGE_SHIFT 12
#define PAGE_SIZE (1UL << PAGE_SHIFT)
#define PAGE_MASK (~(PAGE_SIZE - 1))

#define PAGE_START(addr) ((addr)&PAGE_MASK)
#define PAGE_END(addr) (PAGE_START(addr) + PAGE_SIZE)

int c_test_func(int num)
{
    return num + 10;
}

int new_test_func(int num)
{
    return num * 100;
}

void inject_shellcode()
{
    int status;
    unsigned long vaddr = (unsigned long)&c_test_func;
    unsigned long page_start = PAGE_START(vaddr);
    unsigned long page_end = PAGE_END(vaddr);
    printf("page start : %lx\n", page_start);
    printf("page end   : %lx\n", page_end);
    status = mprotect((void *)page_start, PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
    if (status < 0)
    {
        perror("mprotect err");
        return;
    }
    printf("new_test_func vaddr:0x%lx\n", (unsigned long)&new_test_func);
    
}

int main(int argc, char const *argv[])
{
    inject_shellcode();

    printf("c_test_func vaddr:0x%lx\n", (unsigned long)&c_test_func);
    int ret = c_test_func(10);
    printf("c_test_func ret:%d\n", ret);

    getchar();
    return 0;
}
