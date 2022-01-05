#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 描述
 C 库函数 long int atol(const char *str) 把参数 str 所指向的字符串转换为一个长整数（类型为 long int 型）。

 声明
 下面是 atol() 函数的声明。

 long int atol(const char *str)
 参数
 str -- 要转换为长整数的字符串。
 返回值
 该函数返回转换后的长整数，如果没有执行有效的转换，则返回零。
 * */

int main() {
	long val;
	char str[20];

	strcpy(str, "98993489");
	val = atol(str);
	printf("字符串值 = %s, 长整型值 = %ld\n", str, val);

	strcpy(str, "runoob.com");
	val = atol(str);
	printf("字符串值 = %s, 长整型值 = %ld\n", str, val);

	return(0);
}
