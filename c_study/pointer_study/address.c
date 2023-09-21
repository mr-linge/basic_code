#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
address:
地址是内存的排序,大小由地址总线决定。
地址总线的长度 一般与 与 long 的长度相同, 任何指针存的都是地址、长度都是 sizeof(unsigned long)

&   取符号在内存中保存的位置,即首地址
*/

void func()
{
    printf("Hello,this is a test\n");
}

int main()
{
    printf("func 		                vaddr:%p\n", func);
    printf("func 		                vaddr:0x%lx\n", (unsigned long)func);
    printf("&func 		                vaddr:0x%lx\n", (unsigned long)&func);
    printf("&func 		                vaddr:%p\n", &func);

    printf("%d======================================\n", __LINE__);

    // h 在 heap 中
    char *h = (char *)malloc(0x10);
    memset(h, 'A', 0x10);
    printf("h 		                vaddr:%p\n", h);
    printf("&h 		                vaddr:0x%lx\n", (unsigned long)&h);
    printf("&h 		                vaddr:%p\n", &h);
    printf("&h[0] 	                        vaddr:0x%lx\n", (unsigned long)(&h[0]));

    printf("%d======================================\n", __LINE__);

    // s 在 stack 中
    char s[0x10] = {0};
    memset(s, 'A', 0x10);
    printf("s 		                vaddr:%p\n", s);
    printf("&s 		                vaddr:0x%lx\n", (unsigned long)&s);
    printf("&s 		                vaddr:%p\n", &s);
    printf("&s[0] 	                \tvaddr:0x%lx\n", (unsigned long)(&s[0]));

    func();
    return 0;
}
