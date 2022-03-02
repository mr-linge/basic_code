#include <stdio.h>
#include <unistd.h>
/*
fork调用的一个奇妙之处就是它仅仅被调用一次，却能够返回两次，它可能有三种不同的返回值：
1、在父进程中，fork返回新创建子进程的进程ID；
2、在子进程中，fork返回0；
3、如果出现错误，fork返回一个负值；

fork()的两种用法：
1. 一个父进程希望复制自己，使父子进程同时执行不同的代码段。
比如在网络服务程序中，父进程等待客户端的服务请求。当请求到达时，父进程调用fork()使子进程处理此请求；而父进程继续等待下一个请求。
2. 一个进程要执行一个不同的程序。
这个在shell下比较常见，这种情况下，fork()之后一般立即接exec函数。
 * */
int main() {
	pid_t pid;
	pid = fork();
	if(pid > 0)
	{      
		sleep(1);
		printf("This is father process ...\n");
		printf("current pid: %d\n", getpid());
		//父进程中返回子进程的pid
		printf("fork() return pid: %d\n", pid);
	}
	else if (pid == 0)
	{
		printf("This is child process ...\n");
		printf("current pid: %d\n", getpid());
		printf("fork() return pid: %d\n", pid);
	}
	else
	{
		printf("fork fail\n");
	}
}
