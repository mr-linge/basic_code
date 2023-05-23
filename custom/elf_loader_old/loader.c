#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define config_path "/home/me/Repository/basic_code/custom/elf_loader/exec_file_paths"

int load(char *path)
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

int main(int argc, char **argv)
{
    printf("%s current pid: %d\n", __FILE__, getpid());
    char buffer[0x1000];
    int fd = open(config_path, O_RDWR);
    if (fd < 0)
    {
        perror("open file fail");
    }
    unsigned long size_r = read(fd, buffer, sizeof(buffer));
    if (size_r < 0)
    {
        perror("read file fail");
    }
    int ret = ftruncate(fd, 0);
    if (ret == -1)
    {
        perror("oftruncate");
    }

    close(fd);
    printf("%s\n", buffer);

    if (access(buffer, F_OK) == 0)
    {
        printf("%s exist\n", buffer);
        load(buffer);
    }

    return 0;
}