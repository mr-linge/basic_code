#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

/*
表头文件：#include <unistd.h>
函数原型：int ftruncate(int fd, off_t  length)
函数说明：ftruncate()会将参数fd指定的文件大小改为参数length指定的大小。参数fd为已打开的文件描述词，而且必须是以写入模式打开的文件。如果原来的文件件大小比参数length大，则超过的部分会被删去
返回值： 0 表示成功;当有错误发生时则返回-1, 错误代码存入errno 中
错误原因：errno
          EBADF     参数fd文件描述词为无效的或该文件已关闭
          EINVAL    参数fd为一socket并非文件，或是该文件并非以写入模式打开

注:
length = 0 清空文件
**/

int main()
{
    char *path = "/tmp/test.txt";
    // 打开文件且拥有可写权限
    int fd = open(path, O_CREAT | O_RDWR, 0666);
    if (fd == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        return -1;
    }

    int status = ftruncate(fd, 0);
    if (status == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        return -1;
    }

    close(fd);

    return 0;
}
