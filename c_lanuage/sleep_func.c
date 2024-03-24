#include <stdio.h>
#include <unistd.h>

/*
 * #include <unistd.h>
 *
 * unsigned int sleep(unsigned int seconds);
 *
 * Description:
 * sleep()会令目前的进程暂停, 直到达到参数seconds 所指定的时间, 或是被信号所中断.
 * return value:
 * 若进程暂停到参数seconds 所指定的时间则返回0, 若有信号中断则返回剩余秒数.
 * */

int main(int argc, char *argv[])
{
	int num = 0;
	while (num < 100)
	{
		printf("I am run %d\n", num++);
		int ret = sleep(2);
		printf("sleep return is %d\n", ret);
	}
	return 0;
}
