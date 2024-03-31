# 装载动态库

一个进程可能依赖多个动态库,通常是在进程启动时将必要的动态库装载入进程虚拟内存中,不过也可使用 dlopen dlsym dlclose 在程序运行过程中动态地加载和卸载动态库

## 加载动态库

程序运行过程中动态地加载动态库,操作系统首先为动态库分配物理内存,然后在进程对应的页表项中建立虚拟页和物理页面之间的映射。操作系统中存在一种引用计数机制,每当一个进程加载了动态库(在该进程的页表中进行一次映射)引用计数+1,当一个进程显式卸载(通过dlclose等)动态库或进程退出时引用计数-1,当减少到0时,系统卸载动态库。

## 函数介绍

### dlopen

Header: `#include <dlfcn.h>`
Function: void *dlopen(const char* path, int mode);
参数:
path    动态库路径
mode
        RTLD_LAZY 暂缓决定,等有需要时再解出符号
        RTLD_NOW 立即决定,返回前解除所有未决定的符号
     　　RTLD_LOCAL
     　　RTLD_GLOBAL 允许导出符号
     　　RTLD_GROUP
        RTLD_WORLD
功能: dlopen 调用 mmap 将动态库映射到进程的内存中,返回句柄(handle) 供 dlsym 调用 
返回值:
    　　失败,返回NULL
    　　成功,返回句柄

### dlsym

Header: `#include <dlfcn.h>`
Function: void *dlsym(void* handle, const char * symbol);
参数:
handle          由dlopen打开动态链接库后返回的句柄
symbol          动态库中函数的名称
功能: dlsym 根据动态链接库操作句柄(handle)与符号(symbol) 获取 symbol 在进程中的虚拟地址
返回值: 符号对应的虚拟地址

### dladdr

Header: `#include <dlfcn.h>`
Function: int dladdr(const void *addr, Dl_info *info);
Params:
addr        符号(symbol) 虚拟地址
info        接收 symbol 信息的结构体指针

typedef struct {
    const char *dli_fname;  /* Pathname of shared object that contains address */
    void       *dli_fbase;  /* Base address at which shared object is loaded */
    const char *dli_sname;  /* Name of symbol whose definition overlaps addr */
    void       *dli_saddr;  /* Exact address of symbol named in dli_sname */
} Dl_info;

Description: 根据符号的虚拟地址,查看符号相关信息

Return:
成功 retval > 0
失败 retval <= 0, 错误信息放在 dlerror 里

### dlclose

Header: `#include <dlfcn.h>`
函数原型为: int dlclose (void *handle);
函数描述: dlclose用于关闭指定句柄的动态链接库,只有当此动态链接库的使用计数为0时,才会真正被系统卸载。

### dlerror

Header: `#include <dlfcn.h>`
函数原型: const char *dlerror(void);
函数描述: 当动态链接库操作函数执行失败时,dlerror可以返回出错信息,返回值为NULL时表示操作函数执行成功。

### 注意

函数 dlopen、dlsym、dlclose、dlerror 在 libdl.so或libdl.a库,编译时需要指定依赖库
如 gcc main.c -o main -ldl
