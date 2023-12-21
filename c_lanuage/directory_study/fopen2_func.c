#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

/*
fread() 函数用来从指定文件中读取块数据。所谓块数据，也就是若干个字节的数据，可以是一个字符，可以是一个字符串，可以是多行数据，并没有什么限制。fread() 的原型为：
size_t fread(void *ptr, size_t size, size_t count, FILE *fp);
fwrite() 函数用来向文件中写入块数据，它的原型为：
size_t fwrite(void *ptr, size_t size, size_t count, FILE *fp);
对参数的说明：
ptr 为内存区块的指针，它可以是数组、变量、结构体等。fread() 中的 ptr 用来存放读取到的数据，fwrite() 中的 ptr 用来存放要写入的数据。
size：表示每个数据块的字节数。
count：表示要读写的数据块的块数。
fp：表示文件指针。
理论上，每次读写 size*count 个字节的数据。

size_t 是在 stdio.h 和 stdlib.h 头文件中使用 typedef 定义的数据类型，表示无符号整数，也即非负数，常用来表示数量。

返回值：返回成功读写的块数。与传入的 count 一致表示读取成功，否则失败：
对于 fwrite() 来说，肯定发生了写入错误，可以用 ferror() 函数检测。
对于 fread() 来说，可能读到了文件末尾，可能发生了错误，可以用 ferror() 或 feof() 检测。


int ferror(FILE *stream) 测试给定流 stream 的错误标识符。
参数
stream -- 这是指向 FILE 对象的指针，该 FILE 对象标识了流。
返回值
如果设置了与流关联的错误标识符，该函数返回一个非零值，否则返回一个零值。
例如:
if(ferror(fp))
{
    printf("读取文件：file.txt 时发生错误\n");
}
clearerr(fp);
**/

#define SIZE_COUNT 0x400

int main()
{
    char buff[SIZE_COUNT] = {0};

    FILE *fp = fopen("/tmp/test.ipa", "rb+");
    if (fp == NULL)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }

    FILE *fp2 = fopen("./test.ipa", "ab+");
    unsigned long size_count = 0;
    // 循环将文件 fp 中的内容读取到 buff 中
    while ((size_count = fread(buff, sizeof(char), SIZE_COUNT, fp)) > 0)
    {
        size_count = fwrite(buff, sizeof(char), SIZE_COUNT, fp2); // 将读取的文件写入到另一个地方 ./test.ipa
        if (size_count != SIZE_COUNT && ferror(fp))
        {
            printf("写入文件:./test.txt 时发生错误\n");
        }
        clearerr(fp);
    }

    fclose(fp2);
    fclose(fp);

    return 0;
}