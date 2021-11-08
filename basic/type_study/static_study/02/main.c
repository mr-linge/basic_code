#include <stdio.h>

// Weak Symbol 会被其他地方同名的 Strong Symbol 覆盖
int func_int;
// symbol加前缀,不会被覆盖
static int func_static_int;

void main() {
	foo();
	printf("main --> func_int = %d, func_static_int = %d\n", func_int, func_static_int);
}
