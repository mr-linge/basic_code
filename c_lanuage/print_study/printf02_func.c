#include <stdio.h>

int main()
{
	int num = 4;

	/*
		width 	表示需要输出的位数
		长度不足 width时, 将输出的前面补上0,直到占满指定列宽为止
	**/
	printf("%03d", num);

	printf("\n**************************\n");

	/* 也可以用 * 代替位数,在后面的参数列表中用变量控制输出位数 */
	int len = 3;
	printf("%0*d", len, num);

	int ret_val = printf("\n");
	printf("ret_val:%d\n", ret_val);

	return 0;
}
