#include <stdio.h>

unsigned long func() {
	return 2000;
}

void main() {
	unsigned long l = func();
	printf("%lu\n",l);
}
