ptrace函数的定义   
#include <sys/ptrace.h>   
long ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data);    

四个参数：   
request: 表示要执行的操作类型   
pid: 要操作的目标进程ID   
addr: 要监控的目标内存地址    
data: 保存读取出或者要写入的数据   

// enum __ptrace_request  参数的命令, 会让子进程处于挂起状态   
PTRACE_TRACEME    
PTRACE_PEEKTEXT   
PTRACE_PEEKDATA   
PTRACE_PEEKUSER   
PTRACE_POKETEXT   
PTRACE_POKEDATA   
PTRACE_POKEUSER   
PTRACE_GETREGS    
PTRACE_GETFPREGS    
PTRACE_SETREGS    
PTRACE_SETFPREGS    
PTRACE_SYSCALL    
PTRACE_SINGLESTEP   

// enum __ptrace_request 参数的命令 会结束子进程的挂起状态   
PTRACE_DETACH   结束父进程对子进程的跟踪    
PTRACE_CONT		结束子进程的挂起状态，让子进程继续执行   

父进程结束 后子进程也会结束挂起状态    

所以每次使用 ptrace 函数后，可以使用 ptrace(PTRACE_CONT,.....) 让子程序继续运行   

参考资料：https://man7.org/linux/man-pages/man2/ptrace.2.html
