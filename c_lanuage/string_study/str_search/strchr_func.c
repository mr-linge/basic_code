#include <stdio.h>
#include <string.h>

/*
  描述
  C 库函数 char *strchr(const char *str, int c) 在参数 str 所指向的字符串中搜索第一次出现字符 c（一个无符号字符）的位置。

  声明
  下面是 strchr() 函数的声明。

  char *strchr(const char *str, int c)
  参数
  str -- 要被检索的 C 字符串。
  c -- 在 str 中要搜索的字符。
  返回值
  该函数返回在字符串 str 中第一次出现字符 c 的位置，如果未找到该字符则返回 NULL。
**/

int main()
{
        const char str[] = "http://www.runoob.com";
        const char ch = '.';
        char *ret;
        ret = strchr(str, ch);
        printf("|%c| 之后的字符串是 - |%s|\n", ch, ret);

        return 0;
}
