#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void func()
{
    printf("Hello,this is a test\n");
}

int main()
{
    printf("&func 		vaddr:0x%lx\n", (unsigned long)&func);
    printf("&func 		vaddr:%p\n", &func);
    printf("func 		vaddr:%p\n", func);
    printf("func 		vaddr:0x%lx\n", (unsigned long)func);

    puts("==================================");
    char *p = (char *)malloc(32);
    memset(p, 'A', 32);
    printf("&p 		vaddr:0x%lx\n", (unsigned long)&p);
    printf("&p		vaddr:%p\n", &p);
    printf("p 		vaddr:%p\n", p);
    printf("p 		vaddr:0x%lx\n", (unsigned long)p);

    func();
    return 0;
}
