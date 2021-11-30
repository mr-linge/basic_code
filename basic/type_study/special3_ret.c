#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint8_t * get_message(char *path, unsigned long * len) {
	*len = 10;
	return NULL;
}

void main() {
	unsigned long *len = (unsigned long *)malloc(sizeof(unsigned long));
	get_message(NULL,len);
	printf("%lu\n", *len);
}
