#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
#include <sys/wait.h>
#include <sys/types.h>

funtcion:
pid_t wait(int *status);

Description:
wait()会暂时停止目前进程的执行, 直到有信号来到或子进程结束. 如果在调用wait()时子进程已经结束, 则wait()会立即返回子进程结束状态值. 子进程的结束状态值会由参数status 返回, 而子进程的进程识别码也会一快返回.

params:
int *status
如果不在意结束状态值, 则参数 status 可以设成NULL.如果参数status的值不是NULL，wait就会把子进程退出时的状态取出并存入status，这是一个整数值（int），指出了子进程是正常退出还是被非正常结束的（一个进程也可以被其他进程用信号结束），以及正常结束时的返回值，或被哪一个信号结束的等信息。由于这些信息被存放在一个整数的不同二进制位中，所以用常规的方法读取会非常麻烦，人们就设计了一套专门的宏（macro）来完成这项工作，如下:
1、WIFEXITED(status) 这个宏用来指出子进程是否为正常退出的，如果是，它会返回一个非零值。
（请注意，虽然名字一样，这里的参数status并不同于wait唯一的参数--指向整数的指针status，而是那个指针所指向的整数，切记不要搞混了。）
2、WEXITSTATUS(status) 		当WIFEXITED返回非零值时，我们可以用这个宏来提取子进程的返回值，如果子进程调用exit(5)退出，WEXITSTATUS(status)就会返回5；如果子进程调用exit(7)，WEXITSTATUS(status)就会返回7。请注意，如果进程不是正常退出的，也就是说，WIFEXITED返回0，这个值就毫无意义
3、WIFSIGNALED(status)     	如果子进程因为一个未捕获的信号而终止，它就返回真；否则返回假。
4、WTERMSIG(status)        	如果WIFSIGNALED(status)为真，则可以用该宏获得导致子进程终止的信号代码。
5、WIFSTOPPED(status)      	如果当前子进程被暂停了，则返回真；否则返回假。
6、WSTOPSIG(status)        	如果WIFSTOPPED(status)为真，则可以使用该宏获得导致子进程暂停的信号代码。

return value:
如果执行成功则返回子进程识别码(PID), 如果有错误发生则返回-1. 失败原因存于errno 中。
errno:
	ECHILD：没有等待的子进程；
	EINTR ：未抓住信号。或者该信号未设置，或者为找到该信号;
**/

int main()
{
	pid_t pid;
	int status; // child process exit status.
	pid = fork();
	if (pid > 0)
	{
		sleep(1);
		printf("This is the parent process, wait for child...\n");
		int child_pid = wait(&status); /*阻塞等待子进程*/
		if (child_pid < 0)
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(-1);
		}
		printf("wait return vaule -> child's pid = %d\n", pid);
		printf("wait receieve param -> status : %d\n", status);
		int exit_state = WIFEXITED(status);
		if (exit_state == 0)
		{
			printf("The child process was terminated abnormally.signal %d\n", WTERMSIG(status));
		}
		else
		{
			int exitno = WEXITSTATUS(status);
			printf("child process exit number : %d\n", exitno);
		}
	}
	else if (pid == 0)
	{
		printf("This is the child process. pid = %d\n", getpid());
		exit(5);
	}
	else
	{
		printf("fork fail\n");
	}
	return 0;
}
