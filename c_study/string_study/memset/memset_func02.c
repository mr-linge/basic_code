#include <stdio.h>
#include <string.h>
#include <stdint.h>

void main() {
	int num = 2;
	int len = 10;
	uint8_t s1[] = "aaaaabbbb1";
	memset((s1 + len - num),'=', num);
	puts(s1);
}
