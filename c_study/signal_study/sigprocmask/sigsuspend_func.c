#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/********
  进程执行到sigsuspend时,sigsuspend并不会立刻返回,进程处于TASK_INTERRUPTIBLE状态并立刻放弃CPU,等待UNBLOCK（mask之外的）信号的唤醒 进程在接收到UNBLOCK（mask之外）信号后,调用处理函数,然后把现在的信号集还原为原来的,sigsuspend返回,进程恢复执行

  sigsuspend的原子操作是:
  （1）设置新的mask阻塞当前进程(上面是用wait替换new,即阻塞SIGUSR1信号)
  （2）收到SIGUSR1信号,阻塞,程序继续挂起；收到其他信号,恢复原先的mask(即包含SIGINT信号的)
  （3）调用该进程设置的信号处理函数(程序中如果先来SIGUSR1信号,然后过来SIGINT信号,则信号处理函数会调用两次,打印不同的内容第一次打印SIGINT,第二次打印SIGUSR1,因为SIGUSR1是前面阻塞的)
  （4）待信号处理函数返回,sigsuspend返回了(sigsuspend将捕捉信号和信号处理函数集成到一起了)
 *********/

void handler(int sig) // 信号处理程序
{
	if (sig == SIGINT)
		printf("SIGINT sig");
	else if (sig == SIGQUIT)
		printf("SIGQUIT sig");
	else
		printf("SIGUSR1 sig");
}

int main()
{
	sigset_t new, old, wait; // 三个信号集
	struct sigaction act;
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0); // 可以捕捉以下三个信号:SIGINT/SIGQUIT/SIGUSR1
	sigaction(SIGQUIT, &act, 0);
	sigaction(SIGUSR1, &act, 0);

	sigemptyset(&new);
	sigaddset(&new, SIGINT); // SIGINT信号加入到new信号集中
	sigemptyset(&wait);
	sigaddset(&wait, SIGUSR1);			// SIGUSR1信号加入wait
	sigprocmask(SIG_BLOCK, &new, &old); // 将SIGINT阻塞,保存当前信号集到old中

	// 临界区代码执行

	if (sigsuspend(&wait) != -1) // 程序在此处挂起；用wait信号集替换new信号集即:过来SIGUSR1信  号,阻塞掉,程序继续挂起；过来其他信号,例如SIGINT,则会唤醒程序执行sigsuspend的原子操作注意:如果“sigaddset(&wait, SIGUSR1);”这句没有,则此处不会阻塞任何信号,即过来任何信号均会唤醒程序
		printf("sigsuspend error");
	printf("After sigsuspend");
	sigprocmask(SIG_SETMASK, &old, NULL);

	return 0;
}
