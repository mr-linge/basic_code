#include <stdio.h>

int func_int = 10;
static int func_static_int = 11;

void foo() {
	func_int = 100;
	printf("func_int = %d, func_static_int = %d\n", func_int, func_static_int);
}

