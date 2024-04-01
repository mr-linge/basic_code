#include <stdio.h>

// 类型转换
int main()
{
	char ch1 = 0x81;
	printf("char		 	 ch1:%d\n", ch1);
	unsigned char ch2 = (unsigned char)ch1;
	printf("unsigned char	 ch2:0x%x %d\n", ch2, ch2);

	int num11 = (int)ch1;
	printf("int			  	num11:%d\n", num11);
	int num12 = (int)ch2;
	printf("int			  	num12:%d\n", num12);

	unsigned int num21 = (unsigned int)ch1; // sign 转 unsign 出问题
	printf("unsigned int	num21:0x%x %u\n", num21, num21);
	unsigned int num22 = (unsigned int)ch2;
	printf("unsigned int    num22:0x%x %u\n", num22, num22);

	return 0;
}
