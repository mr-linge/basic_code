#include <stdio.h>
/*
 * static 修饰的 变量和方法
 * 若在不同的 .c 文件里 symbol 名可以相同，此时编译后 symbol 名相同但 st_shndx 不同(所属 segment 不同)
 * 若在同一个 .c 文件、不同的方法里 symbol 名可以相同，此时 st_shndx 也相同,但是 symbol 名后会加上不同的随机变量
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
