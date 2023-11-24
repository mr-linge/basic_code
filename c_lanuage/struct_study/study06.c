#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 定义一个结构体类型
struct Data
{
    char bytes[0x100];
};

int main()
{
    // dt1 是栈中的一段内存
    struct Data dt1 = {0};
    memset((void *)&dt1, 'a', 0x100);
    printf("&dt1            vaddr:0x%lx\n", (unsigned long)&dt1);

    // dt2 heap 中内存
    struct Data *dt2 = (struct Data *)malloc(sizeof(struct Data));
    memset((void *)dt2, 'a', 0x100);
    printf("dt2             vaddr:0x%lx\n", (unsigned long)dt2);

    return 0;
}
