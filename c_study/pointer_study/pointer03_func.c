#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
pointer and array
array 就是通过 pointer 实现的，数组名是一个地址（首元素地址），即是一个指针常量。（不是指针变量）
指针也可以通过下标管理数据
type * p = (type *) malloc(n * sizeof(type));
*(p + i) 等价 p[i]
区别:
数组存储在 Stack 栈上
指针存储在 Heap  堆上
 * */
int main()
{
	long arr[5] = {101, 102, 103, 104, 105};
	printf("arr 	addr: %p\n", arr);
	for (int i = 0; i < (sizeof(arr) / sizeof(long)); i++)
	{
		printf("arr + %d 	= %p \n", i, (arr + i));
		printf("arr[%d] 	= %ld\n", i, arr[i]);
		printf("*(arr + %d) = %ld\n", i, *(arr + i));
		printf("--- end  ---\n");
	}

	printf("\n\n*************** 01 ******************\n\n");
	long *arr2 = (long *)malloc(5 * sizeof(long));
	unsigned long step1 = (unsigned long)(arr2 + 1);
	unsigned long step2 = (unsigned long)(arr2 + 2);
	unsigned long step = step2 - step1;
	printf("step = %ld\n", step);
	memcpy(arr2, arr, 5 * sizeof(long));
	for (int i = 0; i < 5; i++)
	{
		printf("arr2 + %d 		= %p \n", i, (arr2 + i));
		printf("*(arr2 + %d) 	= %ld\n", i, *(arr2 + i));
		printf("arr2[%d] 		= %ld\n", i, arr2[i]);
		printf("--- end  ---\n");
	}

	printf("\n\n*************** 02 ******************\n\n");
	// 字符串以 \0 结束
	char *p = "Hello World!";

	printf("p  					= %p\n", p);
	printf("&(p[0])  			= %p\n", &(p[0]));

	// p[n]  == *(p + (n * sizeof(char)))
	for (int i = 0; i < (strlen(p) - 1); i++)
	{
		printf("p[%d]                      = %c\n", i, p[i]);
		printf("*(p + (%d * sizeof(char))) = %c\n", i, *(p + i));
		printf("--- end  ---\n");
	}

	printf("\n\n*************** 03 ******************\n\n");
	char *p1 = (char *)malloc(16);
	memset(p1, 'a', 15);
	p1[15] = '\0';
	printf("p1  				    = %p\n", p1);
	printf("&(p1[0])  				= %p\n", &(p1[0]));
	for (int i = 0; i < (strlen(p1) - 1); i++)
	{
		printf("p1[%d]                      = %c\n", i, p1[i]);
		printf("*(p1 + (%d * sizeof(char))) = %c\n", i, *(p1 + i));
		printf("--- end  ---\n");
	}
	free(p1);

	return 0;
}
