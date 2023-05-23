#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 header file:
 #include <sys/stat.h>
 #include <unistd.h>

 int fstat(int fildes, struct stat *buf);

Description:
fstat()用来将参数fildes 所指的文件状态, 复制到参数buf 所指的结构中(struct stat). fstat()与stat()作用完全相同, 不同处在于传入的参数为已打开的文件描述词.

retrun value:
执行成功则返回0, 失败返回-1, 错误代码存于errno.
 * */
int main()
{
   struct stat buf;
   int fd;
   fd = open("/etc/passwd", O_RDONLY);
   fstat(fd, &buf);
   printf("/etc/passwd file size : %lld\n", buf.st_size);
   return 0;
}
