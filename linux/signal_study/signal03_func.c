#include <stdio.h>
#include <signal.h>
#include <unistd.h>

/*
   SIG_DFL	默认的信号处理程序。
   当执行程序时，同样是死循环，此时按下Ctrl+c进程停止，因为我们对2号信号采取默认动作处理，系统默认2号信号终止进程。
 * */
int main()
{
    signal(SIGINT, SIG_DFL);
    while (1)
    {
        printf("开始休眠一秒钟...\n");
        sleep(1);
    }
    return 0;
}
