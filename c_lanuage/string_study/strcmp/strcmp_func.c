#include <stdio.h>
#include <string.h>

/*
   strcmp
   比较两个字符串是否相同,返回0相同，否则不相同.
 * */
int main()
{
	char *str_1 = "abc";
	char *str_2 = "abc";
	char *str_3 = "ABC";
	if (strcmp(str_1, str_2) == 0)
	{
		printf("str_1 is equal to str_2. \n");
	}
	else
	{
		printf("str_1 is not equal to str_2. \n");
	}

	if (!strcmp(str_1, str_2))
	{
		printf("equal\n");
	}

	if (strcmp(str_1, str_3) == 0)
	{
		printf("str_1 is equal to str_3\n");
	}
	else
	{
		printf("str_1 is not equalto str_3\n");
	}
	return 0;
}
