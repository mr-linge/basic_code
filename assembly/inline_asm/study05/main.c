#include <stdio.h>
#include <string.h>

union Register
{
    unsigned char bytes[8];
    unsigned long val;
};

void test()
{
    union Register dt1, dt2, dt3 = {0};
    dt1.val = 0x1234;

    /*
    输出约束: C变量接收汇编结果的都是输出约束
    输入约束: 凡是通过 %[name] 或 %n 获取汇编语句之外数据的都是输入约束. 与 ldr str 等汇编指令没有关系

    %[vaddr1_in]   : 取到 vaddr1_in 别名关联的变量
    [%[vaddr1_in]] : 取到 变量%[vaddr1_in] 指向的内在地址
    */
    asm volatile(
        "LDR x0, [%[vaddr1_in]]    \n" // 地址 %[vaddr1_in]处数据 加载到 x0
        "STR x0, [%[vaddr2_in]]    \n" // x0 上的数据存入 地址 %[vaddr2_in]处
        "ADD %[vaddr_out], x0, #0  \n" // x0 + 0 结果存入 地址%[vaddr_out]处
        : [vaddr_out] "=r"(dt3.bytes)
        : [vaddr1_in] "r"(dt1.bytes), [vaddr2_in] "r"(dt2.bytes)
        : "x0");

    printf("dt2:0x%lx\n", dt2.val);
    printf("dt3:0x%lx\n", dt3.val);
}

int main(int argc, char const *argv[])
{
    test();
    return 0;
}
