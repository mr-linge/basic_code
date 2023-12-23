#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>

/*
函数:size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
功能描述:从给定流 stream 读取数据到 ptr 所指向的数组中
参数
ptr         指向带有最小尺寸 size*nmemb 字节的内存块的指针。
size        指定要读取的每个元素的大小，以字节为单位。
nmemb       指定读出元素的个数，每个元素的大小为 size 字节。
stream      指向 FILE 对象的指针，该 FILE 对象指定了一个输入流。
返回值
ret 返回实际读取到的元素个数, ret == 0 时到达文件末尾。

函数:size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
功能描述:把 ptr 所指向的数组中的数据写入到给定流 stream 中。
参数
ptr         指向要被写入的元素数组的指针。
size        指定写入的每个元素的大小，以字节为单位。
nmemb       指定写入的元素的个数，每个元素的大小为 size 字节。
stream      指向 FILE 对象的指针，该 FILE 对象指定了一个输出流。
返回值
ret 返回实际写入的元素个数,如果 ret != nmemb 则会显示一个错误。

int ferror(FILE *stream) 测试给定流 stream 的错误标识符。
参数
stream  这是指向 FILE 对象的指针，该 FILE 对象标识了流。
返回值
如果设置了与流关联的错误标识符，该函数返回一个非零值，否则返回一个零值。
例如:
if(ferror(fp))
{
    printf("读取文件：file.txt 时发生错误\n");
}
clearerr(fp);
**/

const char *source_path = "/tmp/test.ipa";
const char *target_path = "./test.ipa";

#define ELEMENT_COUNT 0x200
#define ELEMENT_SIZE 0x1

int main()
{
    char *buff = (char *)calloc(ELEMENT_COUNT, ELEMENT_SIZE);

    FILE *fp = fopen(source_path, "rb+");
    if (fp == NULL)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }

    FILE *fp2 = fopen(target_path, "wb+");
    if (fp2 == NULL)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }

    unsigned long element_count_fread = 0;
    unsigned long element_count_fwrite = 0;
    // 循环将文件 fp 中的内容读取到 buff 中
    while ((element_count_fread = fread(buff, ELEMENT_SIZE, ELEMENT_COUNT, fp)) != 0)
    {
        unsigned long TMP_COUNT = element_count_fread;                     // 这里为了方便理解才这样写的
        element_count_fwrite = fwrite(buff, ELEMENT_SIZE, TMP_COUNT, fp2); // 将读取的数据写入到另一个地方
        // printf("element_count_fread:%lu,element_count_fwrite:%lu\n", element_count_fread, element_count_fwrite);
        if (element_count_fwrite != TMP_COUNT) // 实际写入元素个数 与 指定元素个数不一样,表示出错
        {
            if (ferror(fp2))
            {
                fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
            }
            clearerr(fp2);
            exit(-1);
        }
        memset(buff, '\0', ELEMENT_COUNT * ELEMENT_SIZE);
    }

    fclose(fp2);
    fclose(fp);
    free(buff);
    return 0;
}