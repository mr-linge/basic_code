#include <stdio.h>

unsigned long add(unsigned long a, unsigned long b);
/*
    .globl	add
add:
    sub	sp, sp, #16
    str	x0, [sp, #8]
    str	x1, [sp]
    ldr	x8, [sp, #8]
    ldr	x9, [sp]
    add	x0, x8, x9
    add	sp, sp, #16
    ret
*/

int main(void)
{
    printf("%ld\n", add(1, 2));
    return 0;
}
