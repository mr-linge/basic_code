#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <time.h>

#define run_log "/home/me/Repository/basic_code/custom/elf_exec/run.log"

void add_log(char *text)
{
    int fd;
    fd = open(run_log, O_WRONLY | O_CREAT | O_APPEND | O_CREAT, 0644);
    if (fd == -1)
    {
        perror("open error");
        exit(EXIT_FAILURE);
    }
    char buf[0x400];
    bzero(buf, 0x400);

    time_t t = time(0);
    char *timestr = asctime(localtime(&t));
    sprintf(buf, "%s pid:%d %s\n", timestr, getpid(), text);
    write(fd, buf, strlen(buf));
    close(fd);
}

int main(int argc, char *argv[])
{
    printf("%s %d current pid: %d\n", __FILE__, __LINE__, getpid());
    add_log("A new elf was loaded!");
    printf("argc = %d\n", argc);
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
    return 0;
}