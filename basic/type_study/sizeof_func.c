#include <stdio.h>
#include <elf.h>


/*
 * 根据定义，size_t是sizeof关键字（注：sizeof是关键字，并非运算符）运算结果的类型。
 * size_t大于等于地址线宽度
 * size_t存在的最大原因可能是因为：地址线宽度历史中经常都是大于数据线宽度的。
 *
 * unsigned long 一般与cpu 地址总线位数 一致
 * */
int main() {
	printf("char    is %lu byte\n", sizeof(char));
	printf("short   is %lu byte\n", sizeof(short));
	printf("int     is %lu byte\n", sizeof(int));
	printf("long    is %lu byte\n", sizeof(long));
	printf("double  is %lu byte\n", sizeof(double));
	unsigned long len = sizeof(size_t);
	printf("size_t  is %lu byte\n", len);
	printf("uint8_t is %lu byte\n", sizeof(uint8_t));

	return 0;
}
