#include <stdio.h>
#include <string.h>

/*
   const 和函数形参
   const 通常用在函数形参中，如果形参是一个指针，为了防止在函数内部修改指针指向的数据，就可以用 const 来限制。

   在C语言标准库中，有很多函数的形参都被 const 限制了，下面是部分函数的原型：
   size_t strlen ( const char * str );
   int strcmp ( const char * str1, const char * str2 );
   char * strcat ( char * destination, const char * source );
   char * strcpy ( char * destination, const char * source );
   int system (const char* command);
   int puts ( const char * str );
   int printf ( const char * format, ... );
 * */

// 使用 const 对形参加以限制
size_t strnchr(const char *str, char ch) {
	int i, n = 0, len = strlen(str);
	for(i=0; i<len; i++) {
		if(str[i] == ch) {
			n++;
		}
	}  
	return n;
}

int main() {
	char *str = "http://www.jobs8.cn";
	char ch = 'w';
	int n = strnchr(str, ch);
	printf("%d\n", n);
	return 0;
}
