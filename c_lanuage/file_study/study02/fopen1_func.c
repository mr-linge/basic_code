#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

/*
header: #include <stdio.h>

function: FILE *fopen(const char * filename, const char * mode);
Description: 打开文件生成文件指针fp, fp 会记录当前I/O的位置,可以对大文件分段读取

params:
filename 是字符串，用来命名文件
访问模式 mode 的值可以是下列值中的一个：
r	打开一个已有的文本文件，允许读取文件。
w	打开一个文本文件，允许写入文件。如果文件不存在，则会创建一个新文件。在这里，您的程序会从文件的开头写入内容。如果文件存在，则该会被截断为零长度，重新写入。
a	打开一个文本文件，以追加模式写入文件。如果文件不存在，则会创建一个新文件。在这里，您的程序会在已有的文件内容中追加内容。
r+	打开一个文本文件，允许读写文件。
w+	打开一个文本文件，允许读写文件。如果文件已存在，则文件会被截断为零长度，如果文件不存在，则会创建一个新文件。
a+	打开一个文本文件，允许读写文件。如果文件不存在，则会创建一个新文件。读取会从文件的开头开始，写入则只能是追加模式。
如果处理的是二进制文件，则需使用下面的访问模式来取代上面的访问模式：
"rb", "wb", "ab", "rb+", "r+b", "wb+", "w+b", "ab+", "a+b"


int fputc(int c, FILE *fp);
Description:
函数 fputc() 把参数 c 的字符值写入到 fp 所指向的输出流中。如果写入成功，它会返回写入的字符，如果发生错误，则会返回 EOF。

int fputs(const char *s, FILE *fp);
Description:
把一个以 null 结尾的字符串写入到流中。
函数 fputs() 把字符串 s 写入到 fp 所指向的输出流中。如果写入成功，它会返回一个非负值，如果发生错误，则会返回 EOF。

int fprintf(FILE *fp,const char *format, ...);
Description:
把一个字符串写入到文件中

int fgetc(FILE * fp);
Description:
fgetc() 函数从 fp 所指向的输入文件中读取一个字符。返回值是读取的字符，如果发生错误则返回 EOF。

char *fgets(char *buf, int n, FILE *fp);
Description:
函数 fgets() 从 fp 所指向的输入流中读取 n - 1 个字符。它会把读取的字符串复制到缓冲区 buf，并在最后追加一个 null 字符来终止字符串。
如果这个函数在读取最后一个字符之前就遇到一个换行符 '\n' 或文件的末尾 EOF，则只会返回读取到的字符，包括换行符。

int fscanf(FILE *fp, const char *format, ...);
Description:
从文件中读取字符串，但是在遇到第一个空格和换行符时，它会停止读取
**/

const char *file_path = "/tmp/test.txt";

void test1()
{
   FILE *fp = fopen(file_path, "w+");
   if (fp == NULL)
   {
      fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
      exit(-1);
   }
   fprintf(fp, "This is testing for fprintf...\n");
   fputs("This is testing for fputs...\n", fp);
   fputs("This is testing for fputs...\n", fp);
   fputs("This is testing for fputs...\n", fp);
   fputs("This is testing for fputs...\n", fp);
   fputs("This is testing for fputs...\n", fp);
   fputs("This is testing for fputs...\n", fp);
   fputs("This is testing for fputs...\n", fp);
   fputs("This is testing for fputs...\n", fp);
   fputs("This is testing for fputs...\n", fp);
   fputs("This is testing for fputs...\n", fp);
   fputs("This is testing for fputs...\n", fp);

   fclose(fp);
}

void test2()
{
   char buff[BUFSIZ] = {0};

   FILE *fp = fopen(file_path, "r");
   if (fp == NULL)
   {
      fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
      exit(-1);
   }

   fscanf(fp, "%s", buff);
   printf("%s:%d %s\n", __FILE__, __LINE__, buff);

   fgets(buff, 0x10, fp);
   printf("%s:%d %s\n", __FILE__, __LINE__, buff);

   // 按行依次读取文件所有内容
   while (fgets(buff, BUFSIZ, fp) != NULL)
   {
      printf("%s:%d %s", __FILE__, __LINE__, buff);
   }
   puts("");
   fclose(fp);
}

int main()
{
   test1();
   puts("================================");
   test2();
   return 0;
}
