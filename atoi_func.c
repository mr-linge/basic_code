#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 #include <stdlib.h>

 C 库函数 int atoi(const char *str) 把参数 str 所指向的字符串转换为一个整数（类型为 int 型）。
 * */
int main() {
   int val;
   char str[20];
   
   strcpy(str, "98993489");
   val = atoi(str);
   printf("字符串值 = %s, 整型值 = %d\n", str, val);

   strcpy(str, "runoob.com");
   val = atoi(str);
   printf("字符串值 = %s, 整型值 = %d\n", str, val);

   return(0);
}
