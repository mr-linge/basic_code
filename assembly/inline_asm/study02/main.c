#include <stdio.h>

/*
汇编代码字符串中通过别名引用C表达式中的变量
*/
void test()
{
    long a = 10, b = 20, result;

    /*
    %[out]          对应        result
    %[in_a]         对应        a
    %[in_b]         对应        b
    */
    asm volatile(
        "ADD %[out], %[in_a], %[in_b]"
        : [out] "=r"(result)
        : [in_a] "r"(a), [in_b] "r"(b));

    printf("The result is: %ld\n", result);
}

int main(int argc, char const *argv[])
{
    test();
    return 0;
}
