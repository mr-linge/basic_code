#include <stdio.h>

/*
int snprintf ( char * str, size_t size, const char * format, ... );

Description:
设将可变参数(...)按照 format 格式化成字符串，并将字符串复制到 str 中，size 为要写入的字符的最大数目，超过 size 会被截断。
(1) 如果格式化后的字符串长度小于等于 size，则会把字符串全部复制到 str 中，并给其后添加一个字符串结束符 \0；
(2) 如果格式化后的字符串长度大于 size，超过 size 的部分会被截断，只将其中的 (size-1) 个字符复制到 str 中，并给其后添加一个字符串结束符 \0，返回值为欲写入的字符串长度。

params:
str    -- 目标字符串。
size   -- 拷贝字节数(Bytes)。
format -- 格式化成字符串。
...    -- 可变参数。

return value:
合并后的字符串总长度
 * */
int main()
{
    char buffer[50];
    char *s = "0123456789";

    // 读取字符串并存储在 buffer 中
    int j = snprintf(buffer, 10, "aa%s\n", s);

    // 输出 buffer及字符数
    printf("string:\n%s\ncharacter count = %d\n", buffer, j);

    return 0;
}
