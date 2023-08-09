#include <stdio.h>

/*
%N             表示操作数约束列表中第N个变量
*/
void test()
{
    long a = 10, b = 20, result;

    /*
    %0         对应        result
    %1         对应        a
    %2         对应        b

    "=r"(result)表示将一个通用寄存器的值赋给变量result
    "r"(a)表示将变量a映射到一个通用寄存器
    */
    asm volatile(
        "ADD %0, %1, %2"
        : "=r"(result)
        : "r"(a), "r"(b));

    printf("The result is: %ld\n", result);
}

int main(int argc, char const *argv[])
{
    test();
    return 0;
}
