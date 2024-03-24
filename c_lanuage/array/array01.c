#include <stdio.h>

int main()
{
	int arr[10]; /* arr 是一个包含 10 个整数的数组 */

	/* 初始化数组元素 */
	for (int i = 0; i < 10; i++)
	{
		arr[i] = i + 100; /* 设置元素 i 为 i + 100 */
	}

	/* 输出数组中每个元素的值 */
	for (int j = 0; j < 10; j++)
	{
		printf("Element[%d] = %d\n", j, arr[j]);
	}
}
