#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

/*
goto   实现从汇编到C代码的跳转
输出操作数约束列表必须为空
*/
void test(long a)
{
    asm volatile goto(
        "cmp %x0, 10\n"
        "b.eq %l[label]\n"
        :
        : "r"(a)
        : "memory"
        : label);

    a += 11;
label:
    printf("%s: a=%ld\n", __func__, a);
}

int main(int argc, char const *argv[])
{
    test(10);
    puts("================================");
    test(11);
    return 0;
}
