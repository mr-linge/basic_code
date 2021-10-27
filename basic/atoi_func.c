#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
   header file: #include <stdlib.h>

function:
int atoi(const char *str);

params:
str -- 要转换为整数的字符串。

return value:
该函数返回转换后的长整数，如果没有执行有效的转换，则返回零。
 * */
int main() {
	int val;
	char str[20];

	strcpy(str, "98993489");
	val = atoi(str);
	if(val == 0) {
		printf("%s is Illegal parameter\n",str);
		exit(-1);
	}
	printf("字符串值 = %s, 整型值 = %d\n", str, val);

	strcpy(str, "www.jobs8.com");
	val = atoi(str);
	if(val == 0) {
		printf("%s is Illegal parameter\n",str);
		exit(-1);
	}
	printf("字符串值 = %s, 整型值 = %d\n", str, val);

	return 0;
}
