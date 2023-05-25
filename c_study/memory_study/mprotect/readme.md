# mprotect
在Linux中，mprotect()函数可以用来修改一段指定内存区域的保护属性。
函数原型如下：

#include <unistd.h>
#include <sys/mmap.h>
int mprotect(const void *start, size_t len, int prot);

mprotect()函数把自start开始的、长度为len的内存区的保护属性修改为prot指定的值。

prot可以取以下几个值，并且可以用"|"将几个属性合起来使用：

1）PROT_READ：表示内存段内的内容可写；

2）PROT_WRITE：表示内存段内的内容可读；

3）PROT_EXEC：表示内存段中的内容可执行；

4）PROT_NONE：表示内存段中的内容根本没法访问。

需要指出的是，指定的内存区间必须包含整个内存页（4K）。区间开始的地址start必须是一个内存页的起始地址，并且区间长度len必须是页大小的整数倍。

如果执行成功，则返回0；如果执行失败，则返回-1，并且设置errno变量，说明具体因为什么原因造成调用失败。错误的原因主要有以下几个：
1）EACCES
该内存不能设置为相应权限。这是可能发生的，比如，如果你 mmap(2) 映射一个文件为只读的，接着使用 mprotect() 标志为 PROT_WRITE。

2）EINVAL

start 不是一个有效的指针，指向的不是某个内存页的开头。

3）ENOMEM

内核内部的结构体无法分配。

4）ENOMEM

进程的地址空间在区间 [start, start+len] 范围内是无效，或者有一个或多个内存页没有映射。 

如果调用进程内存访问行为侵犯了这些设置的保护属性，内核会为该进程产生 SIGSEGV （Segmentation fault，段错误）信号，并且终止该进程。
