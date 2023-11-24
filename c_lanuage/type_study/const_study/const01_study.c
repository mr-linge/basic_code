#include <stdio.h>

/*
 * 常量：
 * 使用const关键字对变量加以限定, 他的值不能直接修改
 *
 * 由于const定义常量从汇编的角度来看，只是给出了对应的内存地址，而不是象#define一样给出的是立即数，所以，const定义的常量在程序运行过程中只有一份拷贝，而#define定义的常量在内存中有若干个拷贝.
 * */

int getNum(){
	return 100;
}

int main() {
	int n = 90;
	const int MaxNum1 = getNum();  //运行时初始化
	const int MaxNum2 = n;  //运行时初始化
	const int MaxNum3 = 80;  //编译时初始化
	printf("%d, %d, %d\n", MaxNum1, MaxNum2, MaxNum3);


	// 通过强制类型转换，将地址赋给变量，再作修改即可以改变const常量值 
	const int i = 0;
	int *p = (int*) &i;
	*p = 100;
	printf("i = %d\n", i);
	
	return 0;
}
