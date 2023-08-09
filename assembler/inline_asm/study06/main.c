#include <stdio.h>

void test(unsigned long param)
{
    unsigned long result = 0;
    asm volatile(
        "mov x0, %[param_in]            \n"
        "add x0, x0, 0x3                \n"
        "mov %[result], x0              \n" // x0 上的数据转移到 %[result]上
        : [result] "=r"(result)
        : [param_in] "r"(param)
        : "x0");

    printf("result:0x%lx\n", result);
}

int main(int argc, char const *argv[])
{
    test(0x123456);
    return 0;
}
