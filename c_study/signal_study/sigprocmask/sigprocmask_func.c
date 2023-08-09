#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/*****
  int sigprocmask(ubt how,const sigset_t*set,sigset_t *oldset);
  sigprocmask设定对信号屏蔽集内的信号的处理方式(阻塞或不阻塞)。

  参数：
  how：用于指定信号修改的方式，可能选择有三种
	SIG_BLOCK//将set所指向的信号集中包含的信号加到当前的信号掩码中。即信号掩码和set信号集进行或操作。
	SIG_UNBLOCK//将set所指向的信号集中包含的信号从当前的信号掩码中删除。即信号掩码和set进行与操作。
	SIG_SETMASK //将set的值设定为新的进程信号掩码。即set对信号掩码进行了赋值操作。

  set：为指向信号集的指针，在此专指新设的信号集，如果仅想读取现在的屏蔽值，可将其置为NULL。

  oldset：也是指向信号集的指针，在此存放原来的信号集。可用来检测信号掩码中存在什么信号。


  返回说明：
  成功执行时，返回0。失败返回-1，errno被设为EINVAL。
 * *******/

void handler(int sig) // 信号处理函数的实现
{
	printf("SIGINT sig");
}

/********
  本来期望pause()之后，来SIGINT信号，可以结束程序；可是，如果当“取消阻塞”和“pause”之间，正好来了SIGINT信号，结果程序因为pause的原因会一直挂起
  解决的方式，当然是sigsuspend()函数了。
 * ********/
int main()
{
	sigset_t new, old;
	struct sigaction act;
	act.sa_handler = handler; // 信号处理函数handler
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGINT, &act, 0); // 准备捕捉SIGINT信号
	sigemptyset(&new);
	sigaddset(&new, SIGINT);

	sigprocmask(SIG_BLOCK, &new, &old); // 将SIGINT信号阻塞，同时保存当前信号集
	printf("Blocked");
	sigprocmask(SIG_SETMASK, &old, NULL); // 取消阻塞
	pause();							  // 让进程暂停直到信号出现

	return 0;
}
