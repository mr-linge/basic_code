#include <stdio.h>
#include <string.h>

/*
Header:         #include <string.h>
Function:       size_t strlen(const char *str)
Description:    计算字符串 str 的长度，直到空结束字符，但不包括\0结束字符

params:
str         要计算长度的字符串

return:
该函数返回字符串的长度
**/

int main()
{
    char *str = "aaa";
    printf("str: %s, len:%lu\n", str, strlen(str));
    return 0;
}
