#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dlfcn.h>

// 间接调用 system
int call_system(const char *cmd)
{
    char *error;
    int retVal;

    // 动态库路径
    char *dylib_path = "/usr/lib/libSystem.dylib";
    // 打开动态库
    void *handle = dlopen(dylib_path, RTLD_GLOBAL | RTLD_NOW);
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, error);
        return -1;
    }

    // 获取 system 地址
    int (*system)(const char *) = dlsym(handle, "system");
    if (!system)
    {
        return -2;
    }

    retVal = system(cmd);
    if (retVal < 0)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        return -3;
    }

    dlclose(handle); // 关闭句柄
    return 0;
}

int main(int argc, char *argv[])
{
    int retVal = 0;
    char *command = NULL;

    command = "ls -l";
    retVal = call_system(command);
    if (retVal < 0)
    {
        puts("some error was occurred !");
    }

    command = "ps aux";
    retVal = call_system(command);
    if (retVal < 0)
    {
        puts("some error was occurred !");
    }

    return 0;
}
