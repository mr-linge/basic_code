#include <stdio.h>
#include <unistd.h>

/*
Header: #include <unistd.h>

定义函数
int readlink(const char * path ,char * buf,size_t bufsiz);

函数说明
readlink()会将参数path的符号连接内容存到参数buf所指的内存空间，返回的内容不是以NULL作字符串结尾，但会将字符串的字符数返回。若参数bufsiz小于符号连接的内容长度，过长的内容会被截断

返回值
执行成功则传符号连接所指的文件路径字符串，失败则返回-1，错误代码存于errno
错误代码
EACCESS 取文件时被拒绝，权限不够
EINVAL 参数bufsiz 为负数<br> EIO I/O 存取错误。<br> ELOOP 欲打开的文件有过多符号连接问题。<br> ENAMETOOLONG 参数path的路径名称太长<br> ENOENT 参数path所指定的文件不存在
ENOMEM 核心内存不足
ENOTDIR 参数path路径中的目录存在但却非真正的目录。<br>
**/

int main()
{
	char dir[0x100] = {0};
	// 返回当前进程镜像所在目录(绝对路径)
	int n = readlink("/proc/self/exe", dir, sizeof(dir));
	if (n < 0)
	{
		perror("readlink");
		return -1;
	}
	printf("dir: %s\n", dir);

	return 0;
}
