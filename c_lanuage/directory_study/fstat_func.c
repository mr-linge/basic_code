#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 header file:
 #include <sys/stat.h>

 int fstat(int fildes, struct stat *buf);

Description:
fstat()用来将参数fildes 所指的文件状态, 复制到参数buf 所指的结构中(struct stat). fstat()与stat()作用完全相同, 不同处在于传入的参数为已打开的文件描述符.

retrun value:
执行成功则返回0, 失败返回-1, 错误代码存于errno.
 * */
int main()
{
   struct stat buf;
   int fd,ret;
   fd = open("/etc/passwd", O_RDONLY);
   ret = fstat(fd, &buf);
   if(ret != 0) {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        return -1;
    }
   printf("/etc/passwd file size : %lld\n", buf.st_size);
   return 0;
}
