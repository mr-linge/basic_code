ptrace函数的定义
#include <sys/ptrace.h>       
long ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data);

四个参数：
request: 表示要执行的操作类型
pid: 要操作的目标进程ID
addr: 要监控的目标内存地址
data: 保存读取出或者要写入的数据详情请参看man手册https://man7.org/linux/man-pages/man2/ptrace.2.html

参考:
https://www.cnblogs.com/mysky007/p/11047943.html
