#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

/*
#include <unistd.h>
int daemon(int nochdir,int noclose)

参数：
当 nochdir为0时，当前目录变为根目录，否则不变；
当 noclose为0时，标准输入、标准输出和错误输出重导向为/dev/null，也就是不输出任何信息，否则照样输出。

返回值：
deamon()调用了fork()，如果fork成功，那么父进程就调用_exit(2)退出，所以看到的错误信息全部是子进程产生的。如果成功函数返回0，否则返回-1并设置errno
*/
int main(int argc, char *argv[])
{
    char strCurPath[0x400];
    printf("current pid: %d\n", getpid());
    if (daemon(1, 1) < 0)
    {
        perror("error daemon...\n");
        exit(1);
    }
    printf("current pid: %d\n", getpid());
    sleep(10);

    if (getcwd(strCurPath, PATH_MAX) == NULL)
    {
        perror("error getcwd\n");
        exit(1);
    }
    printf("current pid: %d\n", getpid());
    printf("%s\n", strCurPath);
    return 0;
}