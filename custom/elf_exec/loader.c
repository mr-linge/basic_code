#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define config_file "/home/me/Repository/basic_code/custom/elf_exec/config.ini"
#define daemon_log "/home/me/Repository/basic_code/custom/elf_exec/daemon.log"
#define BUF_LEN 0x400

static char log_buf[BUF_LEN];

void add_log(char *text)
{
    if (strlen(text) <= 0)
    {
        return;
    }
    int fd;
    fd = open(daemon_log, O_WRONLY | O_CREAT | O_APPEND, 0644);
    time_t t = time(0);
    char *timestr = asctime(localtime(&t));
    char buf[BUF_LEN];
    bzero(buf, BUF_LEN);
    if (fd == -1)
    {
        sprintf(buf, "%s%s %d %s\n", timestr, __FILE__, __LINE__, strerror(errno));
        add_log(buf);
        close(fd);
        exit(EXIT_FAILURE);
    }

    sprintf(buf, "%s%s", timestr, text);
    write(fd, buf, strlen(buf));
    close(fd);
}

int check_config(char *context)
{
    int fd = open(config_file, O_RDWR);
    if (fd < 0)
    {
        bzero(log_buf, BUF_LEN);
        sprintf(log_buf, "%s %d %s %s\n", __FILE__, __LINE__, config_file, strerror(errno));
        add_log(log_buf);
        return -1;
    }
    struct stat buf;
    stat(config_file, &buf);
    if (buf.st_size == 0)
    {
        return -3;
    }
    bzero(log_buf, BUF_LEN);
    sprintf(log_buf, "%s %d %s buf.st_size:%lu\n", __FILE__, __LINE__, config_file, buf.st_size);
    add_log(log_buf);
    int read_ret = read(fd, context, buf.st_size);
    bzero(log_buf, BUF_LEN);
    sprintf(log_buf, "%s %d %s read return:%d\n", __FILE__, __LINE__, config_file, read_ret);
    add_log(log_buf);
    if (read_ret < 0)
    {
        bzero(log_buf, BUF_LEN);
        sprintf(log_buf, "%s %d %s\n", __FILE__, __LINE__, strerror(errno));
        add_log(log_buf);
        close(fd);
        return -2;
    }
    bzero(log_buf, BUF_LEN);
    sprintf(log_buf, "%s %d execute file: %s %lu\n", __FILE__, __LINE__, context, strlen(context));
    add_log(log_buf);
    int ret = ftruncate(fd, 0);
    bzero(log_buf, BUF_LEN);
    if (ret == 0)
    {
        sprintf(log_buf, "%s %d %s has been cleared!\n", __FILE__, __LINE__, config_file);
        add_log(log_buf);
    }
    else
    {
        sprintf(log_buf, "%s %d %s clear fail:%s\n", __FILE__, __LINE__, config_file, strerror(errno));
        add_log(log_buf);
    }
    close(fd);
    return 0;
}

void replace_image(char *filepath)
{
    bzero(log_buf, BUF_LEN);
    char *const argv_new[] = {"param1", "param2", NULL};
    char *const envp_new[] = {"AA=11", "BB=22", NULL};
    sprintf(log_buf, "%s %d pid:%d 'image will replace by '%s'\n", __FILE__, __LINE__, getpid(), filepath);
    add_log(log_buf);
    int ret = execve(filepath, argv_new, envp_new);
    if (ret == -1)
    {
        bzero(log_buf, BUF_LEN);
        sprintf(log_buf, "%s %d execve fail:%s\n", __FILE__, __LINE__, strerror(errno));
        add_log(log_buf);
    }
}

// 调度器,不断fork新的进程并替换镜像
void process_scheduler(char *imagepath)
{
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        sprintf(log_buf, "%s %d new process id: %d\n", __FILE__, __LINE__, getpid());
        add_log(log_buf);
        replace_image(imagepath);
    }
    else if (pid < 0)
    {
        sprintf(log_buf, "%s %d fork:%s\n", __FILE__, __LINE__, strerror(errno));
        add_log(log_buf);
    }
}

// 轮询查看是否有新的待处理任务
void poll_query_task()
{
    char buffer[BUF_LEN];
    while (1)
    {
        bzero(buffer, BUF_LEN);
        int ret = check_config(buffer);
        if (ret < 0)
        {
            sleep(10);
            continue;
            // return;
        }
        bzero(log_buf, BUF_LEN);
        char *path = buffer;
        if (access(buffer, F_OK) == 0)
        {
            sprintf(log_buf, "%s %d %s execute elf file is exist\n", __FILE__, __LINE__, path);
            add_log(log_buf);
            process_scheduler(buffer);
        }
        else
        {
            sprintf(log_buf, "%s %d %s execute elf file is not exist\n", __FILE__, __LINE__, path);
            add_log(log_buf);
        }
        sleep(10);
    }
}

void creat_daemon()
{
    // chdir("/");
    for (int i = 0; i < 3; i++)
    {
        close(i);
    }
    umask(0);

    bzero(log_buf, BUF_LEN);
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        sprintf(log_buf, "%s %d daemon pid: %d\n", __FILE__, __LINE__, getpid());
        add_log(log_buf);
        poll_query_task();
    }
    else if (pid < 0)
    {
        sprintf(log_buf, "%s %d fork fail:%s\n", __FILE__, __LINE__, strerror(errno));
        add_log(log_buf);
    }
}

int main(int argc, char *argv[])
{
    creat_daemon();
    return 0;
}
