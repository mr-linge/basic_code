#include <stdio.h>
#include <string.h>

/*
   pointer and array
   array 就是通过 pointer 实现的，数组名就是首地址指针,指针也可以通过下标管理数据
   type * p = (type *) malloc(n * sizeof(type)); 
   *(p + (i * sizeof(Type))) == p[i]

   区别:
   数组编译时就确定，存储在 Stack栈上
   指针运行时确定，  存储在 Heap堆上 
 * */
int main() {
	long arr[5] = {101,102,103,104,105};
	for(int i = 0; i < (sizeof(arr) / sizeof(long)); i++) {
		printf("arr + %d = %p \n", i, (arr + i));
		printf("arr[%d] = %ld\n", i, arr[i]);
		printf("*(arr + %d) = %ld\n", i, *(arr + i));
		printf("--- end  ---\n");
	}
	
	printf("\n\n*********************************\n\n");
	
	
	// 字符串以 \0 结束
	char * p = "Hello World!";

	printf("p  				= %p\n", p);
	printf("&(p[0])  			= %p\n", &(p[0]));

	// p[n]  == *(p + (n * sizeof(char)))
	for(int i = 0; i < (strlen(p) - 1); i++) {
		printf("p[%d]                      = %c\n", i, p[i]);
		printf("*(p + (%d * sizeof(char))) = %c\n", i, *(p + (i * sizeof(char))));
		printf("--- end  ---\n");
	}

	return 0;
}
