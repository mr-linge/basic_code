#include <stdio.h>

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
