#include <stdio.h>
#include <string.h>

/*
Header: 		#include <string.h>
Function: 		int strncmp(const char *str1, const char *str2, size_t n)
Description: 	把 str1 和 str2 进行比较,最多比较前 n 个字节

params:
str1 		要进行比较的第一个字符串
str2		要进行比较的第二个字符串
n			要比较的最大字符数

return:
< 0		表示 str1 小于 str2
> 0		表示 str2 小于 str1
= 0		表示 str1 等于 str2
**/

int main()
{
	char str1[0x10] = {0};
	char str2[0x10] = {0};
	int ret;

	strcpy(str1, "func10");
	strcpy(str2, "func1");

	ret = strncmp(str1, str2, 5);

	if (ret < 0)
	{
		printf("str1 小于 str2\n");
	}
	else if (ret > 0)
	{
		printf("str2 小于 str1\n");
	}
	else
	{
		printf("str1 等于 str2\n");
	}

	return 0;
}
