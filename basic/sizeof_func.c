#include <stdio.h>

int main(){
    printf("int     is %lu byte\n", sizeof(int));
    // long 一般与cpu 地址总线位数 一致
    printf("long    is %lu byte\n", sizeof(long));
    printf("double  is %lu byte\n", sizeof(double));
    return 0;
}
