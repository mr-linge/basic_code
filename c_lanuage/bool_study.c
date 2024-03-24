#include <stdio.h>

/*
   0 is false
   not 0 is ture
**/

int main()
{
	int b3 = -1;
	int b4 = 0;
	int b5 = 1;
	int b6 = (6 == 6);
	int b7 = (6 != 6);
	int b8 = !(6 != 6);
	if (b3)
	{
		printf("b3 is true, -1 is true\n");
	}
	if (!b4)
	{
		printf("b4 is false, 0 is false\n");
	}
	if (b5)
	{
		printf("b5 is true, 1 is ture\n");
	}
	if (b6)
	{
		printf("b6 is true, (6 == 6) is %d\n", b6);
	}
	if (!b7)
	{
		printf("b7 is false, (6 != 6) is %d\n", b7);
	}
	if (b8)
	{
		printf("b8 is true, !(6 != 6) is %d\n", b8);
	}

	return 0;
}
