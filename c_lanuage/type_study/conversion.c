#include <stdio.h>

// 类型转换
int main()
{
	char ch1 = 0x81;
	printf("ch1:%x %d\n", ch1, ch1);
	unsigned char ch2 = (unsigned char)ch1;
	printf("ch2:%x %d\n", ch2, ch2);

	int num11 = (int)ch1;
	printf("num11:%x %d\n", num11, num11);
	int num12 = (int)ch2;
	printf("num12:%x %d\n", num12, num12);

	unsigned int num21 = (unsigned int)ch1; // sign 转 unsign 出问题
	printf("num21:%x %u\n", num21, num21);
	unsigned int num22 = (unsigned int)ch2;
	printf("num22:%x %u\n", num22, num22);

	return 0;
}
