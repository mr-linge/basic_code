#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
char *fgets( char *buf, int n, FILE *fp );
Description:
函数 fgets() 从 fp 所指向的输入流中读取 n - 1 个字符。它会把读取的字符串复制到缓冲区 buf，并在最后追加一个 null 字符来终止字符串。
如果这个函数在读取最后一个字符之前就遇到一个换行符 '\n' 或文件的末尾 EOF，则只会返回读取到的字符，包括换行符。
**/

#define MAX_LINE 0x400

int main()
{
    char buf[MAX_LINE]; /*缓冲区*/
    FILE *fp;           /*文件指针*/
    int len;            /*行字符个数*/
    fp = fopen("/tmp/test.txt", "r");
    if (fp == NULL)
    {
        perror("fail to read");
        exit(1);
    }
    // 按行依次读取文件所有内容
    while (fgets(buf, MAX_LINE, fp) != NULL)
    {
        len = strlen(buf);
        buf[len - 1] = '\0'; /*去掉换行符*/
        printf("%s %d \n", buf, len - 1);
    }
    return 0;
}