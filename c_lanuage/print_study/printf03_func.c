#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hexdump(void *ptr, unsigned long len)
{
	unsigned char *mem = ptr;
	for (unsigned long i = 0; i < len; i++)
	{
		printf("%02x ", mem[i]);
		if ((i + 1) % 0x10 == 0)
		{
			puts("");
		}
	}
	puts("");
}

int main()
{
	char *mem = malloc(0x100);
	memset(mem, 0x80, 0x100);
	hexdump(mem, 0x100);
	return 0;
}
