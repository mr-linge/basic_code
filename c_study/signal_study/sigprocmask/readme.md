sigemptyset()函数:初始化信号集
头文件:
# include <signal.h>
定义函数:
int sigemptyset(sigset_t *set);
函数说明:sigemptyset()用来将参数set 信号集初始化并清空.
返回值:执行成功则返回0, 如果有错误则返回-1.
错误代码:EFAULT 参数set 指针地址无法存取。

sigaddset()函数:增加一个信号至信号集
头文件:
# include <signal.h>
定义函数:
int sigaddset(sigset_t *set, int signum);
函数说明:sigaddset()用来将参数signum 代表的信号加入至参数set 信号集里.
返回值:执行成功则返回0, 如果有错误则返回-1.
错误代码:
1、EFAULT 参数set 指针地址无法存取。
2、EINVAL 参数signum 非合法的信号编号。

sigdelset()函数:从信号集里删除一个信号
头文件:
# include <signal.h>
定义函数:
int sigdelset(sigset_t * set, int signum);
函数说明:sigdelset()用来将参数signum 代表的信号从参数set 信号集里删除。
返回值:执行成功则返回0, 如果有错误则返回-1.
错误代码:
1、EFAULT 参数set 指针地址无法存取。
2、EINVAL 参数signum 非合法的信号编号。

sigismember()函数:测试某个信号是否已加入至信号
头文件:#include <signal.h>
定义函数:int sigismember(const sigset_t *set, int signum);
函数说明:sigismember()用来测试参数signum 代表的信号是否已加入至参数set 信号集里. 如果信号集里已有该信号则返回1, 否则返回0.
返回值:信号集已有该信号则返回1, 没有则返回0.如果有错误则返回-1.
错误代码:
1、EFAULT 参数set 指针地址无法存取。
2、EINVAL 参数signum 非合法的信号编号。

sigfillset()函数:将所有信号加入至信号集
头文件:#include <signal.h>
定义函数:int sigfillset(sigset_t * set);
函数说明:sigfillset()用来将参数set 信号集初始化, 然后把所有的信号加入到此信号集里.
返回值:执行成功则返回0, 如果有错误则返回-1。
附加说明:EFAULT 参数set 指针地址无法存取。
