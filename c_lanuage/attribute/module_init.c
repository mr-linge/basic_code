#include <stdio.h>

__attribute__((constructor(103))) void pre_proc_3(void)
{
	printf("%s prio:%d\n", __FUNCTION__, 103);
}

__attribute__((destructor(103))) void end_proc_3(void)
{
	printf("%s prio:%d\n", __FUNCTION__, 103);
}

__attribute__((constructor(102))) void pre_proc_2(void)
{
	printf("%s prio:%d\n", __FUNCTION__, 102);
}

__attribute__((destructor(102))) void end_proc_2(void)
{
	printf("%s prio:%d\n", __FUNCTION__, 102);
}

__attribute__((constructor(101))) void pre_proc_1(void)
{
	printf("%s prio:%d\n", __FUNCTION__, 101);
}

__attribute__((destructor(101))) void end_proc_1(void)
{
	printf("%s prio:%d\n", __FUNCTION__, 101);
}

__attribute__((constructor)) void before_main()
{
	printf("%s\n", __FUNCTION__);
}

__attribute__((destructor)) void after_main()
{
	printf("%s\n", __FUNCTION__);
}

int main(int argc, char **argv)
{
	printf("%s\n", __FUNCTION__);
	return 0;
}
