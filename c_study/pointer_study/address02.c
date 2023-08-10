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

    func();
    return 0;
}
