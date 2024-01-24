#include <stdio.h>
#include <string.h>
#include <errno.h>

/*
在c程序中，system函数可以运行命令行，但是只能得到该命令行的int型返回值，并不能获得显示结果。
例如system("ls")只能得到0或非0，如果要获得ls的执行结果，则要通过管道来完成的。
用popen打开一个命令行的管道，然后通过fgets获得该管道传输的内容，也就是命令行运行的结果
*/
void executeCMD(const char *cmd, char *result)
{
    char buf_ps[0x100];
    char ps[0x100] = {0};
    FILE *ptr;
    strcpy(ps, cmd);
    if ((ptr = popen(ps, "r")) != NULL)
    {
        while (fgets(buf_ps, 0x100, ptr) != NULL)
        {
            strcat(result, buf_ps);
            if (strlen(result) > 0x100)
                break;
        }
        pclose(ptr);
        ptr = NULL;
    }
    else
    {
        printf("popen %s error\n", ps);
    }
}

int main()
{
    char *command = "ls -la";
    char buff[0x100] = {0};
    executeCMD(command, buff);
    printf("command:\n%s\n", buff);
    return 0;
}
