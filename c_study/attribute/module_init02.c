#include <stdio.h>

__attribute__((constructor(103))) void pre_proc_1(void)
{
	printf("%d %s\n", __LINE__, __FUNCTION__);
}

__attribute__((destructor(103))) void end_proc_1(void)
{
	printf("%d %s\n", __LINE__, __FUNCTION__);
}

__attribute__((constructor(102))) void pre_proc_2(void)
{
	printf("%d %s\n", __LINE__, __FUNCTION__);
}

__attribute__((destructor(102))) void end_proc_2(void)
{
	printf("%d %s\n", __LINE__, __FUNCTION__);
}

// gcc –Wall xxx.c
int main(int argc, char **argv)
{
	printf("%d %s\n", __LINE__, __FUNCTION__);
	return 0;
}
