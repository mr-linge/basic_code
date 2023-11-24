#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

/*
  sigaction函数原型：int sigaction(int signo,const struct sigaction *restrict act,struct sigaction *restrict oact);
  第一个参数为信号编号，第二个是sigaction结构体，第三个一般为NULL。

  信号安装函数sigaction(int signum,const struct sigaction *act,struct sigaction *oldact)的第二个参数是一个指向 sigaction结构的指针（结构体名称与函数名一样，千万别弄混淆了）。在结构sigaction的实例中，指定了对特定信号的处理，信号所传递的信息，信号处理函数执行过程中应屏蔽掉哪些函数等。当然，此指针也可以为NULL，进程会以默认方式处理信号。以下就简单介绍一下sigaction结构以及一般的用法。
  对于内核头文件而言，struct sigaction 结构体定义在kernel/include/asm/signal.h,此头文件又被kernel/include/linux/signal.h包含。
  对于用户空间的头文件而言，struct sigaction定义在 /usr/include/bits/sigaction.h,此头文件又被/usr/include/signal.h包含，所以应用程序中如果用到此结构，只要#include 即可。注意内核中的定义和应用程序中的定义是不一样的，内核空间的sigaction结构只支持函数类型为 __sighandler_t的信号处理函数，不能处理信号传递的额外信息


  Linux中信号相关的一个结构体struct sigaction主要在sigaction信号安装和sigqueue信号发送时会用到
  struct sigaction {
	 __sighandler_t sa_handler;
	 unsigned long sa_flags;
	 void (*sa_restorer)(void);
	 sigset_t sa_mask;   // mask last for extensibility
  };
  // Here we must cater to libcs that poke about in kernel headers.
  struct sigaction {
	  union {
	  __sighandler_t _sa_handler;
	  void (*_sa_sigaction)(int, struct siginfo *, void *);
	  } _u;
	  sigset_t sa_mask;
	  unsigned long sa_flags;
	  void (*sa_restorer)(void);
  };

  sa_handler的原型是一个参数为int，返回类型为void的函数指针。参数即为信号值，所以信号不能传递除信号值之外的任何信息;
  sa_flags包含了许多标志位，比较重要的一个是SA_SIGINFO，当设定了该标志位时，表示信号附带的参数可以传递到信号处理函数中。即使sa_sigaction指定信号处理函数，如果不设置SA_SIGINFO，信号处理函数同样不能得到信号传递过来的数据，在信号处理函数中对这些信息的访问都将导致段错误。
  sa_mask指定在信号处理程序执行过程中，哪些信号应当被阻塞。默认当前信号本身被阻塞。
  sa_restorer已过时，POSIX不支持它，不应再使用。

  当你的信号需要接收附加信息的时候，你必须给sa_sigaction赋信号处理函数指针，同时还要给sa_flags赋SA_SIGINFO
**/

static void sig_usr(int signum)
{
	if (signum == SIGUSR1)
	{
		printf("SIGUSR1 received\n");
	}
	else if (signum == SIGUSR2)
	{
		printf("SIGUSR2 received\n");
	}
	else
	{
		printf("signal %d received\n", signum);
	}
}

int main()
{
	char buf[512];
	int n;
	struct sigaction sa_usr;
	sa_usr.sa_handler = sig_usr; // 信号处理函数
	sa_usr.sa_flags = 0;

	sigaction(SIGUSR1, &sa_usr, NULL);
	sigaction(SIGUSR2, &sa_usr, NULL);

	printf("My PID is %d\n", getpid());

	while (1)
	{
		if ((n = read(STDIN_FILENO, buf, 511)) == -1)
		{
			if (errno == EINTR)
			{
				printf("read is interrupted by signal\n");
			}
		}
		else
		{
			buf[n] = '\0';
			printf("%d bytes read: %s\n", n, buf);
		}
	}

	return 0;
}
