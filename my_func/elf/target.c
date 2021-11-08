#include <stdio.h>

int global_init_var = 0x84;
char global_init_char = 'A';
long global_init_long = 0xFFFFFFFFFFFFFFF1;
int global_uninit_var;

void func1(int i) {
	printf("res = %d\n", i);
}

int main() {
	static int static_var = 0x85;
	static int static_var2;
	char *s = "AAAA";
	int a = 1;
	int b;
	func1(static_var + static_var2 + a + b);
	return a;
}
