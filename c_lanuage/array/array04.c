#include <stdio.h>

void func(int num_params)
{
	int a[num_params];
	a[0] = 0x11;
	a[1] = 0x11;

	printf("a[0] = %d\n", a[0]);
}

int main()
{
	func(2);
	return 0;
}
