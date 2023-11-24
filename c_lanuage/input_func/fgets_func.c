#include <stdio.h>

/*
   gets跟scanf一样,会从a的首地址开始存放用户输入的字符,存放完毕后,系统会自动在尾部加上一个结束标记\0。
   gets一次只能读取一个字符串,scanf则可以同时读取多个字符串
   gets可以读入包含空格、tab的字符串,直到遇到回车为止;scanf不能用来读取空格、tab
 **/
int main()
{
	//    char b[10];
	//    gets(b);
	//    printf("the string2 : %s\n", b);
	// 使用 fgets() 替换 gets()
	char buffer[0x10];
	fgets(buffer, sizeof(buffer), stdin);
	printf("the string : %s\n", buffer);
	return 0;
}
