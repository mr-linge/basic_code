#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <spawn.h>

/*
posix_spawn 调用系统命令
**/

int run_cmd(char *cmd, char *argv[])
{
    pid_t pid;
    // 这里注意 cmd 也要包含在 argv[0]中传入。
    posix_spawn(&pid, cmd, NULL, NULL, argv, NULL);
    int stat;
    waitpid(pid, &stat, 0);
    printf("run cmd:%s stat:%d\n", cmd, stat);
    return stat;
}

int main(int argc, char *argv[])
{
    // char *command[] = {
    //     "/bin/ps",
    //     "aux",
    //     NULL
    // };
    // run_cmd(command[0],command);

    // cp -r /private/var/mobile/Containers/Data/Application/A97AEEB5-3D7B-47CA-AC40-AC3646226F26/* /tmp/
    char *command[] = {
        "/usr/bin/cp",
        "-r",
        "/tmp/1.txt",
        "/tmp/2.txt",
        NULL};
    run_cmd(command[0], command);
    return 0;
}
