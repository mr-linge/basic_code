#include <stdio.h>
#include <string.h>

/*
   描述
   C 库函数 char *strcpy(char *dest, const char *src) 把 src 所指向的字符串复制到 dest。

   需要注意的是如果目标数组 dest 不够大，而源字符串的长度又太长，可能会造成缓冲溢出的情况。

   声明
   下面是 strcpy() 函数的声明。

   char *strcpy(char *dest, const char *src)
   参数
   dest -- 指向用于存储复制内容的目标数组。
   src -- 要复制的字符串。
   返回值
   该函数返回一个指向最终的目标字符串 dest 的指针。
 * */

int main()
{
  char src[40] = {0};
  char dest[100] = {0};

  strcpy(src, "This is runoob.com");
  strcpy(dest, src);

  printf("最终的目标字符串： %s\n", dest);

  return (0);
}
