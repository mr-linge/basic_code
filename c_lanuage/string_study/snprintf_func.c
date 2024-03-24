#include <stdio.h>
#include <string.h>
#include <errno.h>

/*
Header:         #include <stdio.h>
Function:       int snprintf (char * str, size_t size, const char * format, [arg1, arg2, ... ]);

Description:
设将可变参数(...)按照 format 格式化成字符串，并将字符串复制到 str 中，size 为要写入的字符的最大数目，超过 size 会被截断
(1) 如果 format 长度小于等于 size，则会把字符串全部复制到 str 中，并给其后添加一个字符串结束符 \0
(2) 如果 format 长度大于 size，超过 size 的部分会被截断，只将其中的 (size-1) 个字符复制到 str 中，并给其后添加一个字符串结束符 \0

params:
    str             字符串指针
    size            指定 copy 字符串长度
    format          字符串的格式化模型
    arg1, arg2      所需拼接的参数

return:
   如果成功,返回值为 format 格式化字符串的长度,失败返回-1
**/

int main()
{
    int retVal;
    char text[0x100] = {0};
    char *agr1 = "0123456789";
    char *agr2 = "abcdefghij";

    // 读取字符串并存储在 buffer 中
    retVal = snprintf(text, 10, "%s%s", agr1, agr2);
    if (retVal == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        return -1;
    }
    printf("retVal = %d\n", retVal);
    printf("text:%s\nlength = %lu\n", text, strlen(text));

    return 0;
}
