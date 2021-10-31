#include <stdio.h>

/*
 * 当 const type * p  修饰一片区域时, *(p + n) 只有读的权限
 * */
int main() {
	const long num1 = 10;
	const long * np = &num1;
	printf("check 10 --> *np = %ld, addr = %p\n", *np, np);
	long * np2 = np; // type *  <-  const type *  编译器会报警告 warminag
	printf("check 11 --> *np2 = %ld, addr = %p\n", *np2, np2);
	*np2 = 6666; // 但可以修改原来的数据
	printf("check 12 --> *np = %ld, *np2 = %ld, np2 addr = %p\n", *np, *np2, np2);

	printf("\n\n*******************************\n\n");

	long num = 10;
	long *p1 = &num;
	printf("check 1 -> *p1 = %ld, addr = %p\n", *p1, p1);
	*p1 = 1000;
	printf("check 2 -> *p1 = %ld, addr = %p\n", *p1, p1);
	const long * p2 = p1;// const type * <- type *  是允许的
	*p1 = 2000;
	printf("check 3 -> *p1 = %ld, addr = %p\n", *p1, p1);
	printf("check 4 -> *p2 = %ld, addr = %p\n", *p2, p2);
	//    *p2 = 3000;  // error
	//    printf("check 5 -> *p1 = %ld, addr = %p\n", *p1, p1);
	//    printf("check 6 -> *p2 = %ld, addr = %p\n", *p2, p2);

	printf("\n\n*******************************\n\n");

	long num2 = 20;
	p2 = &num2; // p2 本身是可以指向别的区域,但所指区域也不能 通过 *(p + n) 修改
	printf("check 7 -> *p2 = %ld, addr = %p\n", *p2, p2);
	//*p2 = 3000;  // error
	//printf("check 8 -> *p2 = %ld, addr = %p\n", *p2, p2);

	return 0;
}
