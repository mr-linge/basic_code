#include <stdio.h>
#include <string.h>

/*
   描述
   C 库函数 char *strrchr(const char *str, int c) 在参数 str 所指向的字符串中搜索最后一次出现字符 c（一个无符号字符）的位置。

   声明
   下面是 strrchr() 函数的声明。

   char *strrchr(const char *str, int c)
   参数
   str -- C 字符串。
   c -- 要搜索的字符。以 int 形式传递，但是最终会转换回 char 形式。
   返回值
   该函数返回 str 中最后一次出现字符 c的地址。如果未找到该值，则函数返回一个空指针。
**/

int main()
{
  const char str[] = "https://www.runoob.com";
  printf("str:%s		addr:%p\n", str, str);
  const char ch = '.';

  char *ret = strrchr(str, ch);
  if (ret == NULL)
  {
    puts("no target string!");
    return -1;
  }

  printf("|%c| 之后的字符串是 - |%s|	addr:%p\n", ch, ret, ret);

  return 0;
}
