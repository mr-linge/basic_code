#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/*
open read 和 write 实现将一个文件 复制到另一个地方,而且文件权限不变
**/

const char *source_path = "/tmp/test.ipa";
const char *target_path = "./test.ipa";

int main()
{
    char buff[BUFSIZ] = {0};

    int fd = open(source_path, O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        return -1;
    }

    // int fd2 = open(target_path, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    struct stat buf;
    int status = stat(source_path, &buf);
    if (status == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        return -1;
    }
    int fd2 = open(target_path, O_WRONLY | O_CREAT, buf.st_mode); // 通过 buf.st_mode 保持 两个文件的状态一致
    if (fd2 == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        return -1;
    }

    unsigned long len = 0;
    // 循环将文件 fd 中的内容读取到 buff 中
    while ((len = read(fd, buff, BUFSIZ)) != 0)
    {
        if (len == -1) // I/O 错误
        {
            fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
            break;
        }
        len = write(fd2, buff, len); // 将读取的数据写入到另一个地方 fd2
        // printf("len:%lu\n", len);
        if (len == -1) // I/O 错误
        {
            fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
            break;
        }
        memset(buff, '\0', BUFSIZ);
    }

    close(fd2);
    close(fd);
    return 0;
}