#include <stdio.h>
#include <string.h>

/*
Header: 		#include <string.h>
Function: 		int strcmp(const char *str1, const char *str2)
Description: 	把 str1 和 str2 进行比较

params:
str1 		要进行比较的第一个字符串
str2		要进行比较的第二个字符串

return:
< 0		表示 str1 小于 str2
> 0		表示 str2 小于 str1
= 0		表示 str1 等于 str2
**/

int main()
{
	char *str1 = "abc";
	char *str2 = "abc";
	char *str3 = "ABC";

	if (strcmp(str1, str2) == 0)
	{
		printf("str1 is equal to str2. \n");
	}
	else
	{
		printf("str1 is not equal to str2. \n");
	}

	if (!strcmp(str1, str2))
	{
		printf("equal\n");
	}

	if (strcmp(str1, str3) == 0)
	{
		printf("str1 is equal to str3\n");
	}
	else
	{
		printf("str1 is not equalto str3\n");
	}

	return 0;
}
