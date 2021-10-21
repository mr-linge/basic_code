#include <stdio.h>

int main(){
   int num = 11;
   int * addr = &num;
   // 以16进制形式输出地址
   printf("num address : %p\n", addr);
   printf("num value   : %d\n", *addr);
   return 0;
}
