#include <stdio.h>

__attribute__((constructor)) void before()
{
	printf("%d %s\n", __LINE__, __FUNCTION__);
}

__attribute__((destructor)) void after()
{
	printf("%d %s\n", __LINE__, __FUNCTION__);
}

// gcc –Wall xxx.c
int main(int argc, char **argv)
{
	printf("%d %s\n", __LINE__, __FUNCTION__);

	return 0;
}
