#include <stdio.h>
#include <stdlib.h>

void get_message(unsigned long * len) {
	*len = 10;
}

void main() {
	unsigned long *len = (unsigned long *)malloc(sizeof(unsigned long));
	get_message(len);
	printf("%lu\n", *len);
}
