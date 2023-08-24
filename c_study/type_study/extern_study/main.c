#include <stdio.h>

/*
 * extern int num;
 * 类似于在当前 .c 文件的顶部申明一个未初始化的全局变量 int num;
 * 作用原理: relocation 时 Strong Symbol 覆盖 Weak Symbol
 * */

int max(int x, int y);

int main(void)
{
    int result;
    /*外部变量声明*/
    extern int g_X;
    extern int g_Y;
    result = max(g_X, g_Y);
    printf("the max value is %d\n", result);
    return 0;
}

/*定义两个全局变量*/
int g_X = 10;
int g_Y = 20;

int max(int x, int y)
{
    return (x > y ? x : y);
}
