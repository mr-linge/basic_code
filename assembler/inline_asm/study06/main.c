#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>

#define PAGE_SHIFT 12
#define PAGE_SIZE (1UL << PAGE_SHIFT)
#define PAGE_MASK (~(PAGE_SIZE - 1))

#define PAGE_START(addr) ((addr)&PAGE_MASK)
#define PAGE_END(addr) (PAGE_START(addr) + PAGE_SIZE)

int c_test_func(int num)
{
    return num + 10;
}

union Data
{
    unsigned char bytes[8];
    unsigned long val;
};

void test()
{
    printf("&c_test_func vaddr:0x%lx\n", (unsigned long)&c_test_func);
    // union Data dt;
    // dt.val = 0x1; //(unsigned long)&c_test_func;
    // printf("dt.val:0x%lx\n", dt.val);
    union Data log1;
    bzero(&log1, sizeof(union Data));

    // 0x65 4f5e 677c
    // 0x70 0000 0000
    unsigned long target_vaddr = (unsigned long)&c_test_func;
    unsigned long page_start = PAGE_START(target_vaddr);
    unsigned long page_offset = target_vaddr - page_start;
    printf("page_start : 0x%lx\n", page_start);
    unsigned long result = 0;
    asm volatile(
        "mov x0, #-1    \n"
        "add x0, x0, 0x78c   \n"
        "ADD %[vaddr_out], x0, #0x0  \n"
        : [vaddr_out] "=r"(log1.bytes), [result] "=r"(result)
        : [page_start_in] "r"(page_start), [page_offset_in] "r"(page_offset)
        : "x0");

    printf("log1:0x%lx\n", log1.val);
}

int main(int argc, char const *argv[])
{
    test();
    return 0;
}
