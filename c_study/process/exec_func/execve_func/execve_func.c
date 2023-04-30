#include <stdio.h>
#include <unistd.h>

/*
   头文件：#include <unistd.h>

   定义函数：int execve(const char * filename, char * const argv[], char * const envp[]);

   函数说明：execve()用来执行参数filename 字符串所代表的文件路径, 第二个参数系利用数组指针来传递给执行文件, 最后一个参数则为传递给执行文件的新环境变量数组.

   返回值：如果执行成功则函数不会返回, 执行失败则直接返回-1, 失败原因存于errno 中.

   错误代码：

   EACCES：
   1. 欲执行的文件不具有用户可执行的权限.
   2. 欲执行的文件所属的文件系统是以noexec 方式挂上.
   3. 欲执行的文件或script 翻译器非一般文件.

   EPERM：
   1. 进程处于被追送模式, 执行者并不具备root 权限, 欲执行的文件具有SUID 或SGID 位.
   2. 欲执行的文件所属的文件系统是以nosuid 方式挂上, 欲执行的文件具有SUID 或SGID 位元, 但执行者并不具有root 权限.

   E2BIG 参数数组过大

   ENOEXEC 无法判断欲执行文件的执行文件格式, 有可能是格式错误或无法在此平台执行.

   EFAULT 参数filename 所指的字符串地址超出可存取空间范围.

   ENAMETOOLONG 参数filename 所指的字符串太长.

   ENOENT 参数filename 字符串所指定的文件不存在.

   ENOMEM 核心内存不足

   ENOTDIR 参数filename 字符串所包含的目录路径并非有效目录

   EACCES 参数filename 字符串所包含的目录路径无法存取, 权限不足.

   ELOOP 过多的符号连接

   ETXTBUSY 欲执行的文件已被其他进程打开而且正把数据写入该文件中

   EIO I/O 存取错误

   ENFILE 已达到系统所允许的打开文件总数.

   EMFILE 已达到系统所允许单一进程所能打开的文件总数.

   EINVAL 欲执行文件的ELF 执行格式不只一个PT_INTERP 节区

   EISDIR ELF 翻译器为一目录

   ELIBBAD ELF 翻译器有问题.
 * */
int main(int argc, char *argv[])
{
  printf("file:%s, line:%d, pid:%d\n", __FILE__, __LINE__, getpid());
  const char *filepath = "../target";
  char *const argv_new[] = {"param1", "param2", NULL};
  char *const envp_new[] = {"AA=11", "BB=22", NULL};
  int ret = execve(filepath, argv_new, envp_new);
  if (ret < 0)
  {
    perror("exeve error");
    return -1;
  }

  printf("file:%s, line:%d, pid:%d\n", __FILE__, __LINE__, getpid());
  return 0;
}
