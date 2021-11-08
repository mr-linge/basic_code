#include <stdio.h>

int aa = 1;
static int bb = 2;

void main() {
	int aa = 10;
	static int bb = 20;
	printf("aa = %d, bb = %d\n", aa, bb);
}
