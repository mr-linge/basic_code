#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

void sig_usr(int signum)
{
	if (signum == SIGUSR1)
	{
		printf("SIGUSR1 received\n");
	}
	else if (signum == SIGUSR2)
	{
		printf("SIGUSR2 received\n");
	}
	else if (signum == SIGILL)
	{
		printf("signal %d received\n", signum);
		exit(-1);
	}
}

int main()
{
	char buf[512];
	int n;
	struct sigaction sa_usr;
	sa_usr.sa_handler = sig_usr; // 信号处理函数
	sa_usr.sa_flags = 0;

	sigaction(SIGILL, &sa_usr, NULL);

	int num = 0;
	while (1)
	{
		printf("开始休眠一秒钟...\n");

		if (num == 5) // 5 sec later go here
		{
			// 生成信号
			int ret = raise(SIGILL);
			if (ret != 0)
			{
				printf("错误，不能生成信号\n");
				exit(-1);
			}
		}

		sleep(1);
		num++;
	}

	return 0;
}
