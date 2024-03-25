#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

/*
header: #include <sys/stat.h>

函数原型： int chmod(const char *path, mode_t mode);
模式	                描述
S_ISUID	            执行时的set-user-ID
S_ISGID	            执行时的set-group-ID
S_ISVTX	            saved-text（粘滞位）
S_IRWXU	            用户读、写、执行
S_IRUSR	            用户读
S_IWUSR	            用户写
S_IXUSR	            用户执行
S_IRWXG	            组读、写、执行
S_IRGRP	            组读
S_IWGRP	            组写
S_IXGRP	            组执行
S_IRWXO	            其他人读、写、执行
S_IROTH	            其他人读
S_IWOTH	            其他人写
S_IXOTH	            其他人执行

功能：用于改变文件访问方式
参数：const char *file   为要修改的文件名, mode_t mode  为要修改的权限
返回值：成功  返回0 , 失败  返回-1
**/

const char *path = "./test.ipa";

int main()
{
    int status = chmod(path, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if (status == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
    }

    return 0;
}
