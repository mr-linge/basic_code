#include <stdio.h>

int main() {
	// 定义一个指针p
	char *p;
	// 定义一个数组s存放字符串
	char s[] = "mjklsts";
	// 指针p指向字符串的首字符'm'
	p = s; // 或者 p = &s[0];
	while(*p != '\0'){
		printf("%c ", *p);
		p++;
	}
	printf("\n");
	return 0;
}
