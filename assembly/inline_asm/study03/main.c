#include <stdio.h>

void test()
{
    unsigned long array[] = {2, 3, 5, 6, 8, 7};
    unsigned long length = sizeof(array);
    unsigned long sum = 0;

    asm volatile(
        "MOV x1, #0\n"               // 初始化x1（sum）为0
        "MOV x2, #0\n"               // 初始化x2（索引）为0
        "loop:\n"                    // 设置循环标签
        "LDR x0, [%[array], x2]\n"   // 读取数组元素到x0
        "ADD %x[sum], %x[sum], x0\n" // 将x0（当前数组元素）累加到（sum）
        "ADD x2, x2, #8\n"           // 增加索引（x2）
        "CMP x2, %x[length]\n"       // 比较索引（x2）和数组长度（length）
        "BLT loop\n"                 // 如果索引小于长度，跳回循环开始
        : [sum] "+r"(sum)            // sum = sum + x0  此处 sum 需要读写权限
        : [array] "r"(array), [length] "r"(length)
        : "x0", "x1", "x2", "cc" // x0 x1 x2 状态寄存器 会被改变
    );

    printf("The sum of the array is: 0x%lx\n", sum);
}

int main(int argc, char const *argv[])
{
    test();
    return 0;
}
