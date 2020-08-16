#include <stdio.h>
#include <stdlib.h>
#include "Test.h"

//参考文章:https://blog.csdn.net/qq_42475711/article/details/85224010

int main()
{
    int a = N; //宏常量
    int b = 2;
    int c = 0;

    c = a + b;
    printf("%d\n", c);

    CODE //宏替换代码段

    DoNothing();

    system("pause");
    return 0;
}
