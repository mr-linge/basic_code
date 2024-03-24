#include <stdio.h>

/*
 * 二维数组
 * */
int main()
{
    int a[2][3] = {{2, 2, 3}, {3, 4, 5}};
    int tmp = a[0][1];
    printf("a[0][1] = %d\n", tmp);
    return 0;
}
