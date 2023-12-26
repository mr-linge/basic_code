#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
Header:		#include <string.h>
Function:	char * strdup(const char *s);
Description:
先用 malloc() 分配 与参数s 字符串长长度相同的空间,然后将参数s 字符串的内容复制到该内存地址,然后把该地址返回。
该地址最后可以利用free()来释放

return:
返回 malloc() 分配的内存首地址
**/

int main()
{
	char a[] = "strdup";
	char *str = strdup(a);
	printf("str: %s\n", str);
	free(str);

	return 0;
}
