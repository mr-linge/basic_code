#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

// 创建一个守护进程
void creat_daemon()
{
    pid_t pid;
    pid = fork();
    if (pid > 0)
    {
    }
    else if (pid == 0)
    {
        // chdir("/");
        for (int i = 0; i < 3; i++)
        {
            close(i);
        }
        umask(0);
    }
    else if (pid < 0)
    {
    }
}

int main(int argc, char *argv[])
{
    creat_daemon();
    return 0;
}
