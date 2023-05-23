#include <stdio.h>

int main()
{
	char a = 'C';
	printf("修改前，a的值：%c\n", a);
	// 指针变量p指向变量a
	char *p = &a;
	// 通过指针变量p间接修改变量a的值
	*p = 'Z';
	printf("修改后，a的值：%c\n", a);
	return 0;
}
