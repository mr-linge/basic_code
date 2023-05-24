#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

/*
创建守护进程的的一般步骤：

1、fork()创建子进程，父进程exit()退出
这是创建守护进程的第一步。由于守护进程是脱离控制终端的，因此，完成第一步后就会在Shell终端里造成程序已经运行完毕的假象。之后的所有工作都在子进程中完成，而用户在Shell终端里则可以执行其他命令，从而在形式上做到了与控制终端的脱离，在后台工作。

2、在子进程中调用 setsid() 函数创建新的会话
在调用了 fork() 函数后，子进程全盘拷贝了父进程的会话期、进程组、控制终端等，虽然父进程退出了，但会话期、进程组、控制终端等并没有改变，因此，这还不是真正意义上的独立开来，而 setsid() 函数能够使进程完全独立出来。

3、再次 fork() 一个子进程并让父进程退出。
现在，进程已经成为无终端的会话组长，但它可以重新申请打开一个控制终端，可以通过 fork() 一个子进程，该子进程不是会话首进程，该进程将不能重新打开控制终端。退出父进程。

4、在子进程中调用 chdir() 函数，让根目录 ”/” 成为子进程的工作目录
这一步也是必要的步骤。使用fork创建的子进程继承了父进程的当前工作目录。由于在进程运行中，当前目录所在的文件系统（如“/mnt/usb”）是不能卸载的，这对以后的使用会造成诸多的麻烦（比如系统由于某种原因要进入单用户模式）。因此，通常的做法是让"/"作为守护进程的当前工作目录，这样就可以避免上述的问题，当然，如有特殊需要，也可以把当前工作目录换成其他的路径，如/tmp。改变工作目录的常见函数是chdir。

5、在子进程中调用 umask() 函数，设置进程的文件权限掩码为0
文件权限掩码是指屏蔽掉文件权限中的对应位。比如，有个文件权限掩码是050，它就屏蔽了文件组拥有者的可读与可执行权限。由于使用fork函数新建的子进程继承了父进程的文件权限掩码，这就给该子进程使用文件带来了诸多的麻烦。因此，把文件权限掩码设置为0，可以大大增强该守护进程的灵活性。设置文件权限掩码的函数是umask。在这里，通常的使用方法为umask(0)。

6、在子进程中关闭任何不需要的文件描述符
同文件权限码一样，用fork函数新建的子进程会从父进程那里继承一些已经打开了的文件。这些被打开的文件可能永远不会被守护进程读写，但它们一样消耗系统资源，而且可能导致所在的文件系统无法卸下。
在上面的第二步之后，守护进程已经与所属的控制终端失去了联系。因此从终端输入的字符不可能达到守护进程，守护进程中用常规方法（如printf）输出的字符也不可能在终端上显示出来。所以，文件描述符为0、1和2 的3个文件（常说的输入、输出和报错）已经失去了存在的价值，也应被关闭。

7、守护进程退出处理
当用户需要外部停止守护进程运行时，往往会使用 kill 命令停止该守护进程。所以，守护进程中需要编码来实现 kill 发出的signal信号处理，达到进程的正常退出。
*/

#define daemon_log "/home/me/Repository/basic_code/c_study/process/daemon/daemon.log"
#define BUF_LEN 0x400

void add_log(char *text)
{
    if (strlen(text) <= 0)
    {
        return;
    }
    int fd;
    fd = open(daemon_log, O_WRONLY | O_CREAT | O_APPEND, 0644);
    time_t t = time(0);
    char *time_str = asctime(localtime(&t));
    char buf[BUF_LEN];
    bzero(buf, BUF_LEN);
    if (fd == -1)
    {
        sprintf(buf, "%s%s %d %s\n", time_str, __FILE__, __LINE__, strerror(errno));
        add_log(buf);
        close(fd);
        exit(EXIT_FAILURE);
    }

    sprintf(buf, "%s%s", time_str, text);
    write(fd, buf, strlen(buf));
    close(fd);
}

void creat_daemon()
{
    pid_t pid = fork();
    if (pid > 0)
    {
        printf("This is father process ...\n");
        printf("current pid: %d\n", getpid());
        // 父进程中返回子进程的pid
        printf("child pid: %d\n", pid);
    }
    else if (pid == 0)
    {
        printf("This is child process ...\n");
        printf("current pid: %d\n", getpid());

        // chdir("/");
        for (int i = 0; i < 3; i++)
        {
            close(i);
        }
        umask(0);

        char buf[0x1000];
        while (1)
        {
            bzero(buf, 0x1000);
            sprintf(buf, "%s %d pid:%d This process is looping ...\n", __FILE__, __LINE__, getpid());
            add_log(buf);
            sleep(10);
        }
    }
    else
    {
        printf("fork fail\n");
    }
}

int main()
{
    creat_daemon();
    return 0;
}
