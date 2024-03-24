#include <stdio.h>
#include <string.h>

/*
   C 库函数 char *strncpy(char *dest, const char *src, size_t n) 把 src 所指向的字符串复制到 dest，最多复制 n 个字符。当 src 的长度小于 n 时，dest 的剩余部分将用空字节填充。

   声明
   下面是 strncpy() 函数的声明。

   char *strncpy(char *dest, const char *src, size_t n)
   参数
   dest -- 指向用于存储复制内容的目标数组。
   src -- 要复制的字符串。
   n -- 要从源中复制的字符数。
   返回值
   该函数返回最终复制的字符串。
 * */

int main()
{
	char src[40] = {0};
	char dest[12] = {0};

	strcpy(src, "This is runoob.com");
	strncpy(dest, src, 10);

	printf("最终的目标字符串： %s\n", dest);

	char path[] = "./test/name1.txt.temp";
	char new_path[512] = {0};
	strncpy(new_path, path, strlen(path) - 5);
	printf("\n%s====\n", new_path);
	return (0);
}
