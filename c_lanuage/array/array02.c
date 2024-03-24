#include <stdio.h>

/*
 * type arr[n];
 * sizeof(arr) ==  n * sizeof(type)
 * */
int main()
{
	// 可以省略 n, 开辟空间后直接赋值
	int arr[] = {0x0, 0x1, 0x2, 0x3, 0x4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
	unsigned long len = (unsigned long)(sizeof(arr) / sizeof(int));
	for (int i = 0; i < len; i++)
	{
		printf("arr[%X] = %d\t", i, arr[i]);
	}

	printf("\n\n************************\n\n");

	int n = 10;
	int arr2[n]; // 开辟 n * sizeof(int) 个空间
	unsigned long l2 = sizeof(arr2);
	unsigned long l3 = n * sizeof(int);
	printf("sizeof(arr2)		= %ld\n", l2);
	printf("n * sizeof(int)		= %ld\n", l3);

	return 0;
}
