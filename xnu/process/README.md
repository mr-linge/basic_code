# Process 进程

进程相关

## DYLD_INSERT_LIBRARIES 指定加载动态库

// 设置当前的环境变量
export DYLD_INSERT_LIBRARIES=/Applications/liba.dylib

// 程序启动时加载动态库
DYLD_INSERT_LIBRARIES=/Applications/liba.dylib /Applications/tracee

## dyld 链接动态库

在iOS系统中,每个程序依赖的动态库都需要通过dyld（位于/usr/lib/dyld）一个一个加载到内存,然而,很多系统库几乎是每个程序都会用到的,如果在每个程序运行的时候都重复的去加载一次,势必造成运行缓慢,为了优化启动速度和提高程序性能,共享缓存机制就应运而生。所有默认的动态链接库被合并成一个大的缓存文件,放到/System/Library/Caches/com.apple.dyld/目录下,按不同的架构保存分别保存着

/System/Library/Frameworks/ 系统库被删了
