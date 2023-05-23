#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define config_path "/home/me/Repository/basic_code/custom/elf_loader/exec_file_paths"
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

int load_elf(char *path)
{
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        printf("This is child process ...\n");
        printf("current pid: %d\n", getpid());
        char *const argv_new[] = {"param1", "param2", NULL};
        char *const envp_new[] = {"AA=11", "BB=22", NULL};
        int ret = execve(path, argv_new, envp_new);
        if (ret < 0)
        {
            perror("exeve error");
            return -1;
        }
    }
    else if (pid < 0)
    {
        printf("fork fail\n");
    }

    return 0;
}

int check_config(char *buffer)
{
    char log_buf[0x1000];
    bzero(log_buf, 0x1000);
    int fd = open(config_path, O_RDWR);
    if (fd < 0)
    {
        bzero(log_buf, 0x1000);
        sprintf(log_buf, " pid:%d open file fail\n", getpid());
        add_log(log_buf);
        return -1;
    }
    struct stat buf;
    stat(config_path, &buf);
    unsigned long size_r = read(fd, buffer, buf.st_size);
    if (size_r < 0)
    {
        sprintf(log_buf, " pid:%d size_r < 0\n", getpid());
        add_log(log_buf);
        close(fd);
        return -1;
    }
    int ret = ftruncate(fd, 0);
    if (ret == -1)
    {
        perror("oftruncate");
    }

    // buffer[buf.st_size] = '\0';
    // printf("%s\n", buffer);
    close(fd);
    if (access(buffer, F_OK) == 0)
    {
        printf("%s exist\n", buffer);
        sprintf(log_buf, " pid:%d %s\n", getpid(), buffer);
        add_log(log_buf);
    }
    else
    {
        sprintf(log_buf, " pid:%d %s not exist\n", getpid(), buffer);
        add_log(log_buf);
        // close(fd);
        return -2;
    }
    // close(fd);
    return 0;
}

int main(void)
{
    char path_buf[0x1000];
    if (check_config(path_buf) == 0)
    {
        printf("path:%s\n", path_buf);
    }
    char buffer[] = "/home/me/Repository/basic_code/custom/elf_loader/test";
    if (access(buffer, F_OK) == 0)
    {
        printf("%s exist\n", buffer);
    }
    else
    {
        printf("%s not exist\n", buffer);
    }

    return 0;
}