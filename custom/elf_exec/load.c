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
#include <pthread.h>

#define config_file "/home/me/Repository/basic_code/custom/elf_exec/config.ini"
#define daemon_log "/home/me/Repository/basic_code/custom/elf_exec/daemon.log"

pthread_mutex_t mutex;

static char log_buf[0x400];

void add_log(char *text)
{
    pthread_mutex_lock(&mutex);
    if (strlen(text) <= 0)
    {
        return;
    }
    int fd;
    fd = open(daemon_log, O_WRONLY | O_CREAT | O_APPEND | O_CREAT, 0644);
    time_t t = time(0);
    if (fd == -1)
    {
        // perror("open error");
        sprintf(log_buf, "%s %d %s\n", __FILE__, __LINE__, strerror(errno));
        add_log(log_buf);
        exit(EXIT_FAILURE);
    }
    char buf[0x400];
    bzero(buf, 0x400);

    char *timestr = asctime(localtime(&t));
    sprintf(buf, "%s%s", timestr, text);
    write(fd, buf, strlen(buf));
    close(fd);
    pthread_mutex_unlock(&mutex);
}

int check_config(char *context)
{
    char buffer[0x400];
    bzero(buffer, 0x400);
    int fd = open(config_file, O_RDWR);
    if (fd < 0)
    {
        // perror("open file fail");
        sprintf(log_buf, "%s: %s\n", config_file, strerror(errno));
        add_log(log_buf);
        return -1;
    }
    struct stat buf;
    stat(config_file, &buf);
    if (buf.st_size == 0)
    {
        return -3;
    }
    bzero(log_buf, 0x400);
    sprintf(log_buf, "%s %d %s buf.st_size:%lu\n", __FILE__, __LINE__, config_file, buf.st_size);
    add_log(log_buf);
    int file_size = read(fd, buffer, buf.st_size);
    bzero(log_buf, 0x400);
    sprintf(log_buf, "%s %d file_size:%d\n", __FILE__, __LINE__, file_size);
    add_log(log_buf);
    if (file_size < 0)
    {
        // perror("read file fail");
        bzero(log_buf, 0x400);
        sprintf(log_buf, "%s %d %s\n", __FILE__, __LINE__, strerror(errno));
        add_log(log_buf);
        close(fd);
        return -2;
    }
    bzero(log_buf, 0x400);
    sprintf(log_buf, "%s %d %s file_size:%d\n", __FILE__, __LINE__, config_file, file_size);
    add_log(log_buf);
    memcpy(context, buffer, buf.st_size);
    bzero(log_buf, 0x400);
    sprintf(log_buf, "%s %d %s %lu %lu\n", __FILE__, __LINE__, context, strlen(buffer), strlen(context));
    add_log(log_buf);
    int ret = ftruncate(fd, 0);
    bzero(log_buf, 0x400);
    if (ret == 0)
    {
        sprintf(log_buf, "%s %d %s was empty now!\n", __FILE__, __LINE__, config_file);
        add_log(log_buf);
    }
    else
    {
        sprintf(log_buf, "%s %d %s\n", __FILE__, __LINE__, strerror(errno));
        add_log(log_buf);
    }
    close(fd);
    return 0;
}

int execve_image(char *filepath)
{
    pid_t pid;
    pid = fork();
    bzero(log_buf, 0x400);
    if (pid == 0)
    {
        // printf("This is child process ...\n");
        // printf("%s %d current pid: %d\n", __FILE__, __LINE__, getpid());
        char *const argv_new[] = {"param1", "param2", NULL};
        char *const envp_new[] = {"AA=11", "BB=22", NULL};
        int ret = execve(filepath, argv_new, envp_new);
        if (ret < 0)
        {
            sprintf(log_buf, "%s %d %s\n", __FILE__, __LINE__, strerror(errno));
            add_log(log_buf);
            // perror("exeve error");
            return -1;
        }
        else
        {
            sprintf(log_buf, "%s %d new process id: %d\n", __FILE__, __LINE__, getpid());
            add_log(log_buf);
        }
    }
    else if (pid < 0)
    {
        sprintf(log_buf, "%s %d %s\n", __FILE__, __LINE__, strerror(errno));
        add_log(log_buf);
        // printf("fork fail\n");
    }

    return 0;
}

// void load_elf()
// {
//     char buffer[0x400];
//     bzero(buffer, 0x400);
//     int ret = check_config(buffer);
//     if (ret < 0)
//     {
//         sleep(10);
//     }
//     printf("execute file path:%s\n", buffer);
//     printf("%s %d file size:%lu\n", __FILE__, __LINE__, strlen(buffer));
//     if (access(buffer, F_OK) == 0)
//     {
//         printf("%s exist\n", buffer);
//         execve_image(buffer);
//     }
//     else
//     {
//         printf("%s not exist\n", buffer);
//     }
// }

void creat_daemon()
{
    // chdir("/");
    for (int i = 0; i < 3; i++)
    {
        close(i);
    }
    umask(0);

    char buffer[0x400];
    bzero(buffer, 0x400);
    while (1)
    {
        int ret = check_config(buffer);
        char *tmp = buffer;
        bzero(log_buf, 0x400);
        sprintf(log_buf, "%s %d ret:%d %s\n", __FILE__, __LINE__, ret, tmp);
        add_log(log_buf);
        if (ret < 0)
        {
            sleep(10);
            bzero(buffer, 0x400);
            continue;
            // return;
        }
        bzero(log_buf, 0x400);
        // printf("execute file path:%s\n", buffer);
        // printf("%s %d file size:%lu\n", __FILE__, __LINE__, strlen(buffer));
        if (access(buffer, F_OK) == 0)
        {
            sprintf(log_buf, "%s %d %s exist\n", __FILE__, __LINE__, tmp);
            add_log(log_buf);
            execve_image(tmp);
        }
        else
        {
            // printf("%s not exist\n", buffer);
            sprintf(log_buf, "%s %d %s not exist\n", __FILE__, __LINE__, tmp);
            add_log(log_buf);
        }
        sleep(10);
        bzero(buffer, 0x400);
    }
    return;
}

int main(int argc, char *argv[])
{
    // 初始化锁
    pthread_mutex_init(&mutex, NULL);

    // bzero(log_buf, 0x400);
    // sprintf(log_buf, "%s %d current pid: %d\n", __FILE__, __LINE__, getpid());
    // add_log(log_buf);
    // bzero(log_buf, 0x400);
    // check_config();
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        sprintf(log_buf, "%s %d current pid: %d\n", __FILE__, __LINE__, getpid());
        add_log(log_buf);
        creat_daemon();
    }
    else if (pid < 0)
    {
        // sprintf(log_buf, "fork: %s\n", strerror(errno));
        // add_log(log_buf);
    }
    // load_elf();

    return 0;
}
