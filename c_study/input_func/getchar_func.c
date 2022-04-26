#include <stdio.h>

/***************
  描述
  C 库函数 int getchar(void) 从标准输入 stdin 获取一个字符（一个无符号字符）。这等同于 getc 带有 stdin 作为参数。

  声明
  下面是 getchar() 函数的声明。

  int getchar(void)
  参数
  NA
  返回值
  该函数以无符号 char 强制转换为 int 的形式返回读取的字符，如果到达文件末尾或发生读错误，则返回 EOF。
 * ****************/

int main ()
{
	char c;

	printf("请输入字符：");
	c = getchar();

	printf("输入的字符：");
	putchar(c);
	
	puts("");

	return(0);
}
