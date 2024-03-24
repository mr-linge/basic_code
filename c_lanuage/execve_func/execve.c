#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/*
Header:     #include <unistd.h>
Function:   int execve(const char * filename, char * const argv[], char * const envp[]);
Parameters:
    filename          文件路径
    argv              数组指针
    envp              环境变量

Description: 加载进程镜象

Return:
      成功不会返回
      失败返回-1, 失败原因存于 errno 中
**/

int main(int argc, char *argv[])
{
  printf("file:%s, line:%d, pid:%d\n", __FILE__, __LINE__, getpid());
  const char *filepath = "../target";
  char *const argv_new[] = {"param1", "param2", NULL};
  char *const envp_new[] = {"AA=11", "BB=22", NULL};
  int ret = execve(filepath, argv_new, envp_new);
  if (ret < 0)
  {
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
    return -1;
  }

  printf("file:%s, line:%d, pid:%d\n", __FILE__, __LINE__, getpid());
  return 0;
}
