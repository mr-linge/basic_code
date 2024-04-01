#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 汇编实现 memcpy 功能
void memcpy_by_asm(unsigned long dst, unsigned long src, unsigned long counter)
{
    unsigned long end = src + counter;
    // printf("dst:0x%lx\nsrc:0x%lx\ncounter:0x%lx\n\nend:0x%lx\n", dst, src, counter, end);
    unsigned long result = 0;
    asm volatile(
        "mov        x0,         %[src]          \n"
        "mov        x1,         %[dst]          \n"
        "loop:                                  \n"
        "ldr        x3,         [x0]            \n"
        "str        x3,         [x1]            \n"
        "add        x0,         x0,         #8  \n"
        "add        x1,         x1,         #8  \n"
        "cmp        x0,         %[end]          \n"
        "blt        loop                        \n"
        "mov        %[result],  x0              \n"
        : [result] "=r"(result)
        : [src] "r"(src), [dst] "r"(dst), [end] "r"(end) // input list, readonly
        : "x0", "x1", "x2", "x3", "cc", "memory");

    printf("result  :0x%lx\n", result);
}

int main(int argc, char const *argv[])
{
    const char test_str[] = "Save time and access relevant information in a convenient, fast, and reliable way using the leading engineering information discovery platform. Engineering Village is a powerful search platform that is essential for researchers, students, and faculty.";
    char *src_mem = (char *)malloc(0x100);
    memcpy(src_mem, test_str, strlen(test_str));
    // printf("src_mem vaddr:%p\n", &src_mem);
    // printf("src_mem:\n%s\n", src_mem);

    char *dst_mem = (char *)malloc(0x100);
    memset(dst_mem, 'B', 0x100);
    // printf("dst_mem vaddr:%p\n", &dst_mem);

    unsigned long len = 0x8; // 由于 ldr 每次读取64位数据,这里字符串的长度需要是 8 的倍数
    memcpy_by_asm((unsigned long)dst_mem, (unsigned long)src_mem, len);
    printf("dst_mem:\n");
    for (int i = 0; i < 0x50; i++)
    {
        printf("%02X ", (int)dst_mem[i]);
        if ((i + 1) % 0xF == 0)
        {
            puts("");
        }
    }
    puts("");
    printf("dst_mem:\n%s\n", dst_mem);

    return 0;
}
