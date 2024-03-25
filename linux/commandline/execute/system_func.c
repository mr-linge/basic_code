#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*
头文件:    #include <stdlib.h>
函数:     int system(const char * string);
函数说明:  调用fork()产生子进程,由子进程来调用/bin/sh-c string来执行参数string字符串所代表的命令,此命令执行完后随即返回原调用的进程。
在调用system()期间SIGCHLD 信号会被暂时搁置,SIGINT和SIGQUIT 信号则会被忽略。

返回值:
-1    出现错误
=0    调用成功但是没有出现子进程
>0    成功退出的子进程的id
**/

int main()
{
    char *command = "ls -l";
    int ret = system(command);
    printf("system ret:%d\n", ret);
    if (ret < 0)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
    }

    return 0;
}
