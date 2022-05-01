#include <stdio.h>
#include <signal.h>
#include <unistd.h>

/*
 忽略此信号（SIG_IGN）
 当执行程序时，陷入死循环，此时按下Ctrl+c进程并不会停止，因为我们对Ctrl+c产生的2号SIGINT信号采取了忽略处理，若要停止进程可用Ctrl+z（SIGQUIT)
 * */
int main()
{
    signal(SIGINT,SIG_IGN);
    while(1)
    {
	printf("开始休眠一秒钟...\n");
        sleep(1);
    }
    return 0;
}
