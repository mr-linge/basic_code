#include <stdio.h>

int main()
{
    int a = 10;   // 10 进制
    int b = 0x10; // 16 进制
    int c = 0b10; // 2  进制
    printf("a = %d, b = %d, c= %d\n", a, b, c);
    printf("a = %x, b = %x, c= %x\n", a, b, c);

    return 0;
}