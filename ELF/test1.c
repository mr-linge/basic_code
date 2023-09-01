#include <stdio.h>

int a_var_global = 84; // 已初始化全局变量 -> .data
int b_var_global;      // 未初始化全局变量 -> .bbs
int h_var_global[256]; // 未初始化全局变量 -> .bbs 并且h不占有真正的内存
static int d_var = 86; // .data
static int e_var;      // .bss

void func_example(int i) // 指令函数地址 -> .text
{
    printf("example %d\n", i);
}

int main(void) // 指令函数地址 -> .text
{
    static int s_var_1 = 85; // 已初始化静态变量 -> .data       符号加函数名前缀 main.s_var_1
    static int s_var_2;      // 未初始化静态变量 -> .bbs        符号加函数名前缀 main.s_var_2
    int c_var = 1;           // 栈
    int b_var;               // 栈
    func_example(s_var_1 + s_var_2 + c_var + b_var + d_var + e_var);

    return 0;
}
