#include <stdio.h>

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

	/*
	 * sizeof :  return (long unsigned int)
	 *
	 * 地址总线的长度 一般与 是 long 的长度
	 * */
	printf("num addr size   : %lu\n", sizeof(num_addr));
	printf("a   addr size   : %lu\n", sizeof(a_addr));

	return 0;
}
