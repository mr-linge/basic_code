#include <stdio.h>
/*
 * static
 * static 修饰后 无论是var还是function,对应的符号前都会加上文件名、方法名(如果在方法内)
 * static int num; (是全局变量)
 * 等价于
 * static int myfile_num
 *
 * static int mun; (在函数内,假设在main函数内)
 * 等价于
 * static int myfile_main_num
 * 
 * 因此 static 修饰的 变量和方法 不会出现符号冲突,但作用范围只在当前 .c 文件内
 * */
void func();

void main() {
	static int a  = 10;
	printf("main a = %d\n", a);
	func();
}

void func() {
	static int a = 11;
	printf("func a = %d\n", a);
}
