## C语言--使用dlopen、dlsym、dlclose运行时装载动态库

一个可执行程序可能与多个动态库有关联，通常是在程序运行时将必要的动态库装载入进程实体(内存中);

另外一种，则是使用dlopen/dlsym/dlclose来动态地将动态库装载到当前进程实体中.

加载动态链接库，首先为共享库分配物理内存，然后在进程对应的页表项中建立虚拟页和物理页面之间的映射。

你可以认为系统中存在一种引用计数机制， 每当一个进程加载了共享库（在该进程的页表中进行一次映射），引用计数加一；

一个进程显式卸载（通过dlclose等）共享库或进程退出时，引用计数减一,当减少到0时，系统卸载共享库。

(1)打开动态链接库：dlopen，函数原型void *dlopen (const char *filename, int flag); dlopen用于打开指定名字(filename)的动态链接库，并返回操作句柄。

(2)取函数执行地址：dlsym，函数原型为: void *dlsym(void *handle, char *symbol); dlsym根据动态链接库操作句柄(handle)与符号(symbol)，返回符号对应的函数的执行代码地址。当handle == NULL 时，获取进程已经加载的系统模块里的函数，在x86-64 上是 libc-2.31.so 这个模块里的函数.

(3)关闭动态链接库：dlclose，函数原型为: int dlclose (void *handle); dlclose用于关闭指定句柄的动态链接库，只有当此动态链接库的使用计数为0时,才会真正被系统卸载。

(4)动态库错误函数：dlerror，函数原型为: const char *dlerror(void); 当动态链接库操作函数执行失败时，dlerror可以返回出错信息，返回值为NULL时表示操作函数执行成功。

通常的三段式就是：先打开（dlopen），然后获得需要的函数（dlsym），然后调用函数，最后关闭（dlclose）


注:如果你的程序中使用dlopen、dlsym、dlclose、dlerror 显示加载动态库，gcc编译时需要设置链接选项 -ldl
如 gcc main.c -o main -ldl
