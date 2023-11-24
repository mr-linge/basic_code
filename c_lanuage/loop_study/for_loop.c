#include <stdio.h>

int main()
{
	int num_params = 10;
	int i;
	for (i = 0; i < num_params && i < 8; i++)
	{
		printf("i = %d\n", i);
	}
	printf("i = %d\n", i);

	return 0;
}
