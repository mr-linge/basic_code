#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

/*
汇编代码字符串中通过别名引用C表达式中的变量
*/
void test()
{
    long a = 10, b = 20, result;

    /*
    %x[out]          对应        result
    %x[in_a]         对应        a
    %x[in_b]         对应        b  
    */
    asm volatile(
        "ADD %x[out], %x[in_a], %x[in_b]"
        : [out] "=r"(result)
        : [in_a] "r"(a), [in_b] "r"(b));

    printf("The result is: %ld\n", result);
}

int main(int argc, char const *argv[])
{
    test();
    return 0;
}
