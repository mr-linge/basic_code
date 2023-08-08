#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>

int c_test_func(int num)
{
    return num + 10;
}

union Data
{
    unsigned char bytes[8];
    unsigned long val;
};

void test()
{
    printf("&c_test_func vaddr:0x%lx\n", (unsigned long)&c_test_func);
    union Data dt;
    dt.val = 0x1; //(unsigned long)&c_test_func;
    printf("dt.val:0x%lx\n", dt.val);
    union Data log1;
    bzero(&log1, sizeof(union Data));
    union Data log2;
    bzero(&log2, sizeof(union Data));

    /*
    输入约束: 凡是通过 %[name] 或 %n 获取汇编语句之外数据的都是输入约束. 与 ldr str 等汇编指令没有关系
    输出约束: C变量接收汇编结果的都是输出约束

    %[vaddr1_in]   : 取到 vaddr1_in 别名关联的变量
    [%[vaddr1_in]] : 取到 变量%[vaddr1_in] 指向的内在地址
    */
    asm volatile(
        "LDR x0, [%[vaddr1_in]]    \n"
        "STR x0, [%[vaddr2_in]]    \n"
        "ADD %[vaddr_out], x0, #0  \n"
        : [vaddr_out] "=r"(log1.bytes)
        : [vaddr1_in] "r"(dt.bytes), [vaddr2_in] "r"(log2.bytes)
        : "x0");

    printf("log1:0x%lx\n", log1.val);
    printf("log2:0x%lx\n", log2.val);
}

int main(int argc, char const *argv[])
{
    test();
    return 0;
}
