#include <stdio.h>

/*
   sprintf() 函数在 C 语言中把一个整数转换成字符串
   顾名思义，它用于将任何值打印成字符串。这个函数提供了一个将整数转换成字符串的简单方法。它的工作原理与 printf() 函数相同，但它并不直接在控制台打印一个值，而是返回一个格式化的字符串。返回值通常会被丢弃，但如果在操作过程中出现错误，它会返回 -1。

   sprintf() 语法
   int sprintf(char *str, const char *format, [arg1, arg2, ... ]);
   str 是一个指向 char 数据类型的指针。
   format 是用来显示输出类型和占位符。
   arg1, arg2 是要转换为字符串的整数。

   返回值:
   返回拼接后的字符长度
 * */
int main(void) {
	int number;
	char text[0x400]; 

	printf("Enter a number: ");
	scanf("%d", &number);

	int ret = sprintf(text, "This string is %d,over.", number);   
	printf("ret = %d\n",ret);
	printf("\nYou have entered: %s\n", text);

	return 0;
}
