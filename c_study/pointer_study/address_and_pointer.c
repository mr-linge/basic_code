#include <stdio.h>

/*
 * address:
 * 地址是内存的排序，大小由地址总线决定。
 * 地址总线的长度 一般与 与 long 的长度相同, 任何指针存的都是地址、长度都是 sizeof(unsigned long)
 *
 * pointer:
 * 指针存储的是地址。
 * type * p;  					// 定义指针
 * p = (type *) malloc(n * sizeof(type)); 	// 将指针指向一片内存区域
 * p[n] 等价于  *(p + (n * (sizeof(type))))  	即指针指向的内存区域 第n个 type 类型数据
 *
 * *pi 		指针pi所指向的数据,以 pi 为起始后 sizeof(type) * 8 位 地址所对应的数据，即内存地址在 [pi, (pi + (8 * sizeof(type)))] 区域所对应的数据
 * */
int main(){
	int num = 11;
	int * num_addr = &num;
	// 以16进制形式输出地址
	printf("num address : %p\n", num_addr);
	printf("num value   : %d\n", *num_addr);

	char a = 'i';
	char * a_addr = &a;
	printf("a address   : %p\n", a_addr);
	printf("a value	    : %c\n", *a_addr);

	printf("\n\n*********************\n\n");

	/*
	 * sizeof :  return (unsigned long int)
	 *
	 * 地址总线的长度 一般与 与 long 的长度相同, 任何指针存的都是地址、长度都是 sizeof(unsigned long)
	 * */
	printf("unsigned long size 	: %lu\n", sizeof(unsigned long));
	printf("a_addr size     	: %lu\n", sizeof(a_addr));
	printf("char * size     	: %lu\n", sizeof(char *));
	printf("short * size     	: %lu\n", sizeof(short *));
	printf("num_addr size   	: %lu\n", sizeof(num_addr));
	printf("int * size      	: %lu\n", sizeof(int *));

	return 0;
}
