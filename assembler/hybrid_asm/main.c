#include <stdio.h>

unsigned long add(unsigned long a, unsigned long b);
/*
    .globl	add
add:
    sub     sp, sp, #0x10                   // 获取栈空间
    stp     x29, x30, [sp]

    add     x0, x0, x1

    ldp     x29, x30, [sp]                  // 从start函数栈恢复x29,x30寄存器
    add     sp, sp, #0x10
    ret
*/

int main(void)
{
    printf("%ld\n", add(11, 22));
    return 0;
}
