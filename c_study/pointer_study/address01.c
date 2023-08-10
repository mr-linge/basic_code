#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
address:
地址是内存的排序,大小由地址总线决定。
地址总线的长度 一般与 与 long 的长度相同, 任何指针存的都是地址、长度都是 sizeof(unsigned long)
 
&   取地址
*/

int main()
{
    char *p = (char *)malloc(0x10);
    memset(p, 'A', 0x10);
    printf("&p 		vaddr:0x%lx\n", (unsigned long)&p);
    printf("&p 		vaddr:%p\n", &p);
    printf("p 		vaddr:%p\n", p);
    printf("&p[0] 	vaddr:0x%lx\n", (unsigned long)(&p[0]));

    return 0;
}
