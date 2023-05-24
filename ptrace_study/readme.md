# ptrace函数	
#include <sys/ptrace.h>		
long ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data);    

参数：   
request: 表示要执行的操作类型   
pid: 	 要操作的目标进程ID   
addr:  	 要监控的目标内存地址    
data:  	 保存读取出或者要写入的数据   

返回值:		
成功返回0。错误返回-1。errno被设置。		
错误:	
EPERM		特殊进程不可以被跟踪或进程已经被跟踪。	
ESRCH		指定的进程不存在	
EIO		请求非法	

功能详细描述	
1)   PTRACE_TRACEME	
**形式：**ptrace(PTRACE_TRACEME,0 ,0 ,0)	
**描述：**本进程被其父进程所跟踪。其父进程应该希望跟踪子进程。		

2)  PTRACE_PEEKTEXT, PTRACE_PEEKDATA	
**形式：**	
ptrace(PTRACE_PEEKTEXT, pid, addr, 0)	
ptrace(PTRACE_PEEKDATA, pid, addr, 0)	
**描述：**从内存地址中读取一个word，pid表示被跟踪的子进程，内存地址由addr给出，返回值为读到的数据。在Linux（i386）中用户代码段与用户数据段重合所以读取代码段和数据段数据处理是一样的。	

3)  PTRACE_POKETEXT, PTRACE_POKEDATA	
**形式：**	
ptrace(PTRACE_POKETEXT, pid, addr, data)	
ptrace(PTRACE_POKEDATA, pid, addr, data)	
**描述：**往内存地址中写入一个word。pid表示被跟踪的子进程，内存地址由addr给出，data为所要写入的数据。	

4)  TRACE_PEEKUSR	
**形式：**	
ptrace(PTRACE_PEEKUSR, pid, addr, data)		
**描述：**	
从USER区域中读取一个word，pid表示被跟踪的子进程，USER区域地址由addr给出，data为用户变量地址用于返回读到的数据。USER结构为core文件的前面一部分，它描述了进程中止时的一些状态，如：寄存器值，代码、数据段大小，代码、数据段开始地址等。在Linux（i386）中通过PTRACE_PEEKUSER和PTRACE_POKEUSR可以访问USER结构的数据有寄存器和调试寄存器。		

5)  PTRACE_POKEUSR	
**形式：**	
ptrace(PTRACE_POKEUSR, pid, addr, data)		
**描述：**	
往USER区域中写入一个word，pid表示被跟踪的子进程，USER区域地址由addr给出，data为需写入的数据。	

6)   PTRACE_CONT	
**形式：**	
ptrace(PTRACE_CONT, pid, 0, signal)	
**描述：**	
继续执行。pid表示被跟踪的子进程，signal为0则忽略引起调试进程中止的信号，若不为0则继续处理信号signal。	 

7)  PTRACE_SYSCALL	
**形式：**	
ptrace(PTRACE_SYS, pid, 0, signal)	
**描述：**	
继续执行。pid表示被跟踪的子进程，signal为0则忽略引起调试进程中止的信号，若不为0则继续处理信号signal。与PTRACE_CONT不同的是进行系统调用跟踪。在被跟踪进程继续运行直到调用系统调用开始或结束时，被跟踪进程被中止，并通知父进程。		

8)   PTRACE_KILL	
**形式：**ptrace(PTRACE_KILL,pid)	
**描述：**杀掉子进程，使它退出。pid表示被跟踪的子进程。		

9)   PTRACE_SINGLESTEP		
**形式：**ptrace(PTRACE_KILL, pid, 0, signle)	
**描述：**设置单步执行标志，单步执行一条指令。pid表示被跟踪的子进程。signal为0则忽略引起调试进程中止的信号，若不为0则继续处理信号signal。当被跟踪进程单步执行完一个指令后，被跟踪进程被中止，并通知父进程。	

10)  PTRACE_ATTACH	
**形式：**ptrace(PTRACE_ATTACH,pid)	
**描述：**跟踪指定pid 进程。pid表示被跟踪进程。被跟踪进程将成为当前进程的子进程，并进入中止状态。	

11)  PTRACE_DETACH	
**形式：**ptrace(PTRACE_DETACH,pid)	
**描述：**结束跟踪。 pid表示被跟踪的子进程。结束跟踪后被跟踪进程将继续执行。	

12)  PTRACE_GETREGS	
**形式：**ptrace(PTRACE_GETREGS, pid, 0, data)		
**描述：**读取寄存器值，pid表示被跟踪的子进程，data为用户变量地址用于返回读到的数据。此功能将读取所有17个基本寄存器的值。	

13)  PTRACE_SETREGS
**形式：**ptrace(PTRACE_SETREGS, pid, 0, data)		
**描述：**设置寄存器值，pid表示被跟踪的子进程，data为用户数据地址。此功能将设置所有17个基本寄存器的值。	

14)  PTRACE_GETFPREGS	
**形式：**ptrace(PTRACE_GETFPREGS, pid, 0, data)	
**描述：**读取浮点寄存器值，pid表示被跟踪的子进程，data为用户变量地址用于返回读到的数据。此功能将读取所有浮点协处理器387的所有寄存器的值。	

15)  PTRACE_SETFPREGS		
**形式：**ptrace(PTRACE_SETREGS, pid, 0, data)		
**描述：**设置浮点寄存器值，pid表示被跟踪的子进程，data为用户数据地址。此功能将设置所有浮点协处理器387的所有寄存器的值。	


注意:
PTRACE_ATTACH    会让进程处于中止状态. 这个函数调用后必须用 wait 等待子进程的中止信号，收到子进程的信号后才能进行其他操作
PTRACE_CONT      让子进程继续运行
PTRACE_DETACH    结束对子进程的跟踪,结束跟踪后被跟踪进程将继续执行
除了上面这三条命令外，其他所有命令使用前子进程必须处于中止状态。否则可能会出错，像子进程 stopped，或 读、写数据失效。

获取子进程状态 使用 wait 函数
int status;
wait(&status);
if(WIFSTOPPED(status)) 判断子程是否处于中止状态
当子进程处于中止状态后
WSTOPSIG(status) 获取子进程中止信号
WSTOPSIG(status) == SIGCONT 则是 PTRACE_ATTACH 导致的子进程中止
WSTOPSIG(status) == SIGTRAP 则是 由于子进程运行到调试断点后产生的 信号

参考资料：https://man7.org/linux/man-pages/man2/ptrace.2.html
