#include "test.h"

unsigned long printf_vaddr = 0;

void hello()
{
	printf("%s, go here ...\n", __FILE__);
}
