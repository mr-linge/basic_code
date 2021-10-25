#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
/*
   header files: #include <sys/types.h>/<sys/wait.h>

function:
pid_t waitpid(pid_t pid, int *status, int options);

Description:
waitpid()会暂时停止目前进程的执行, 直到有信号来到或子进程结束. 如果在调用wait()时子进程已经结束, 则wait()会立即返回子进程结束状态值. 子进程的结束状态值会由参数status 返回, 而子进程的进程识别码也会一快返回. 如果不在意结束状态值, 则参数status 可以设成NULL. 参数pid为欲等待的子进程识别码.

params:
pid_t pid
	pid<-1 等待进程组识别码为pid绝对值的任何子进程.
	pid=-1 等待任何子进程, 相当于wait().
	pid=0 等待进程组识别码与目前进程相同的任何子进程.
	pid>0 等待任何子进程识别码为pid 的子进程.
int *status
	WIFEXITED(status)	如果子进程正常结束，它就返回真；否则返回假。
	WEXITSTATUS(status)	如果WIFEXITED(status)为真，则可以用该宏取得子进程exit()返回的结束代码。
	WIFSIGNALED(status)	如果子进程因为一个未捕获的信号而终止，它就返回真；否则返回假。
	WTERMSIG(status)	如果WIFSIGNALED(status)为真，则可以用该宏获得导致子进程终止的信号代码。
	WIFSTOPPED(status)	如果当前子进程被暂停了，则返回真；否则返回假。
	WSTOPSIG(status)	如果WIFSTOPPED(status)为真，则可以使用该宏获得导致子进程暂停的信号代码。
int option 可以为0 或下面的组合：
	WNOHANG			如果pid指定的子进程没有结束，则waitpid()函数立即返回0，而不是阻塞在这个函数上等待；如果结束了，则返回该子进程的进程号。
	WUNTRACED		如果子进程进入暂停状态，则马上返回。

return value:
	如果执行成功则返回子进程识别码(PID), 如果有错误发生则返回-1. 失败原因存于errno 中。
error no：
	ECHILD	：没有可等待的pid号；
	EINTR	：未抓住信号。或者该信号未设置，或者为找到该信号
	EINVAL	：options未被初始化


注意:
wait() 和waitpid() 的区别
wait等待第一个终止的子进程，而waitpid可以通过pid参数指定等待哪一个子进程。当pid=-1、option=0时，waitpid函数等同于wait，可以把wait看作waitpid实现的特例。
waitpid函数提供了wait函数没有提供的三个功能：
1、waitpid等待一个特定的进程，而wait则返回任一终止子进程的状态 。
2、waitpid提供了一个 wait的非阻塞版本，有时希望取得一个子进程的状态， 但不想进程阻塞。
3、waitpid支持作业控制。
 * */
int main(void)
{
	pid_t pid;
	int status; // child process exit status.
	pid = fork();
	if (pid > 0) {
		sleep(1);
		printf("This is the parent process, wait for child...\n");
		int status;
		int child_pid = waitpid(pid, &status, 0);  /*阻塞等待子进程*/
		if(child_pid < 0) {
                        perror("wait fail");
                        exit(-1);
                }
                printf("wait return vaule -> child's pid = %d\n", pid);
                printf("wait receieve param -> status : %d\n", status);
                int exit_state = WIFEXITED(status);
                if(exit_state == 0) {
                        printf("The child process was terminated abnormally.signal %d\n", WTERMSIG(status));
                }else {
                        int exitno = WEXITSTATUS(status);
                        printf("child process exit number : %d\n", exitno);
                }
	}
 	else if(pid == 0)
        {
                printf("This is the child process. pid = %d\n", getpid());
                exit(5);
        }else
        {
                printf("fork fail\n");
        }
	return 0;
}
