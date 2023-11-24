#include <stdio.h>

int main()
{
	int num = 0;
loop:
	if (num < 10)
	{
		printf("num = %d\n", num);
		num++;
		goto loop;
	}
	return 0;
}
