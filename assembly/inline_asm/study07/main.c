#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// adrp 实测 功能是把低12位置0,得到所在page
void test(unsigned long vaddr)
{
    printf("vaddr :0x%lx\n", vaddr);
    unsigned long result1 = 0;
    unsigned long result2 = 0;
    asm volatile(
        "adrp	x0,                 test                                        \n"
        "mov    %[result1],         x0                                          \n"
        "add    %[result2],         x0,             :lo12:test                  \n"
        : [result1] "=r"(result1), [result2] "=r"(result2)
        : [vaddr] "r"(vaddr)
        : "x0", "cc", "memory");

    printf("result1:0x%lx\n", result1);
    printf("result2:0x%lx\n", result2);
}

int main(int argc, char const *argv[])
{
    unsigned char *dst_mem = (unsigned char *)malloc(0x100);
    memset(dst_mem, '\0', 0x100);

    test((unsigned long)test);
    printf("dst_mem:\n");
    for (int i = 0; i < 50; i++)
    {
        printf("%02X ", (int)dst_mem[i]);
    }
    puts("");
    return 0;
}
