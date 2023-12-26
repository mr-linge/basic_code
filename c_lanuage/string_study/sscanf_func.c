#include <stdio.h>
#include <string.h>

/*
Header:     #include <stdio.h>
Function:   int sscanf(const char *str, const char *format, ...);
Description: 从 str 读取 format 格式化字符串

params:
参数
str         数据的源
format      格式化数据模式

return:
成功返回成功匹配和赋值的个数,如果到达文件末尾或发生读错误，则返回 -1
**/

int main()
{
    char buff[0x100] = {0};
    char *tmp = "Yeah, We are in 2024, everythin will be success!";
    memcpy(buff, tmp, strlen(tmp));

    char str1[0x10], str2[0x10], str3[0x10];
    int year;

    unsigned long retVal = sscanf(buff, "Yeah, %s %s %s %d", str1, str2, str3, &year);
    printf("retVal:%lu\n", retVal);
    if (retVal < 0)
    {
        printf("%s:%d match fail\n", __FILE__, __LINE__);
    }

    printf("Read String1 |%s|\n", str1);
    printf("Read String2 |%s|\n", str2);
    printf("Read String3 |%s|\n", str3);
    printf("Read Integer |%d|\n", year);

    return (0);
}
