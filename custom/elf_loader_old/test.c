#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define run_log "/home/me/Repository/basic_code/custom/elf_loader/run.log"

int add_log(char *text)
{
    time_t t;
    char log_buf[0x1000];
    int fd = open(run_log, O_WRONLY | O_CREAT | O_APPEND | O_CREAT, 0644);
    if (fd == -1)
    {
        perror("open error");
        exit(EXIT_FAILURE);
    }
    t = time(0);
    char *timestr = asctime(localtime(&t));
    bzero(log_buf, 0x1000);
    strcat(log_buf, timestr);
    sprintf(log_buf, " %s \n", text);
    write(fd, log_buf, strlen(log_buf));
    close(fd);
}

int main(int argc, char **argv)
{
    char log_buf[0x1000];
    bzero(log_buf, 0x1000);

    printf("%s current pid: %d\n", __FILE__, getpid());
    printf("A new ELF file is loader!\n");

    sprintf(log_buf, " pid:%d A new ELF file is loader!\n", getpid());
    add_log(log_buf);

    return 0;
}