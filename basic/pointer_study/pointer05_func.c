#include <stdio.h>

int main() {
	long l1 = 2000;
	long * p1 = &l1;
	printf("p1 = %p\n", p1);
	long ** p2 = &p1;
//	long ** p2 = &&l1; // error 数据只有存储到内存中 才会有地址
	printf("p2 = %p\n", p2);
	
	return 0;
}
