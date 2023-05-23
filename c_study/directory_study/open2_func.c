#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    char *path = "/tmp/test.txt";
    // 打开文件且拥有可写权限
    int fd = open(path, O_CREAT | O_RDWR, 0666);
    if (fd == -1)
    {
        perror("open file fail");
    }
    int ret = ftruncate(fd, 0);
    if (ret == -1)
    {
        perror("oftruncate");
    }
    close(fd);
    return 0;
}
