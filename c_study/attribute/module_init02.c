#include <stdio.h>

__attribute__((constructor(103))) void pre_proc_1(void)
{
	printf("%s %d\n", __FUNCTION__, 103);
}

__attribute__((destructor(103))) void end_proc_1(void)
{
	printf("%s %d\n", __FUNCTION__, 103);
}

__attribute__((constructor(102))) void pre_proc_2(void)
{
	printf("%s %d\n", __FUNCTION__, 102);
}

__attribute__((destructor(102))) void end_proc_2(void)
{
	printf("%s %d\n", __FUNCTION__, 102);
}

int main(int argc, char **argv)
{
	printf("%s\n", __FUNCTION__);
	return 0;
}
