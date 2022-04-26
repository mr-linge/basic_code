#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/syscall.h>

// ARM64 寄存器结构体
struct pt_regs
{
	long long uregs[34];
};
#define ARM_r0 uregs[0]	 // 存储R0寄存器的值，函数调用后的返回值会存储在R0寄存器中.调用函数时保存第 1 个参数
#define ARM_r1 uregs[1]	 // 调用函数时保存第 2 个参数
#define ARM_r2 uregs[2]	 // 调用函数时保存第 3 个参数
#define ARM_r3 uregs[3]	 // 调用函数时保存第 4 个参数
#define ARM_r4 uregs[4]	 // 调用函数时保存第 5 个参数
#define ARM_r5 uregs[5]	 // 调用函数时保存第 6 个参数
#define ARM_r6 uregs[6]	 // 调用函数时保存第 7 个参数
#define ARM_r7 uregs[7]	 // 调用函数时保存第 8 个参数
#define ARM_fp uregs[29] // X29 is the frame pointer register (FP). 用来定位有效的栈帧记录。帧指针寄存器，存放当前过程调用栈帧的起始地址，可使用FP别名引用
#define ARM_lr uregs[30] // 链接寄存器，用于保存过程调用的返回地址，可使用LR别名引用
/*
X30 is the link register (LR). The branch-and-link instructions that store a return address in the link register (BL and BLR), setting the register X30 to PC+4.
Calls to subroutines, where it is necessary for the return address to be stored in the link register(X30).
*/
#define ARM_sp uregs[31]   // 栈指针寄存器，指向当前堆栈的栈顶
#define ARM_pc uregs[32]   // 存储当前的执行地址
#define ARM_cpsr uregs[33] // 存储状态寄存器的值

#define CPSR_T_MASK (1u << 5)

#define LONGSIZE  sizeof(long)


int long_size = sizeof ( long );

void reverse( char *str)
{  
	int i, j;
	char temp;
	for (i = 0, j = strlen (str) - 2; i <= j; ++i, --j) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
	}

}

void getdata(pid_t pid, long addr,
		char *str, int len)
{  
	char *laddr;
	int i, j;
	union u {
		long val;
		char chars[LONGSIZE];
	}data;
	i = 0;
	j = len / long_size;
	laddr = str;
	while (i < j) {
		data.val = ptrace(PTRACE_PEEKDATA,
				pid, addr + i * 4,
				NULL);
		memcpy (laddr, data.chars, long_size);
		++i;
		laddr += long_size;
	}
	j = len % long_size;
	if (j != 0) {
		data.val = ptrace(PTRACE_PEEKDATA,
				pid, addr + i * 4,
				NULL);
		memcpy (laddr, data.chars, j);
	}
	str[len] = '\0' ;
}

void putdata(pid_t pid, long addr,
		char *str, int len)
{  
	char *laddr;
	int i, j;
	union u {
		long val;
		char chars[8];
	}data;
	i = 0;
	j = len / long_size;
	laddr = str;
	while (i < j) {
		memcpy (data.chars, laddr, long_size);
		ptrace(PTRACE_POKEDATA, pid,
				addr + i * 4, data.val);
		++i;
		laddr += long_size;
	}
	j = len % long_size;
	if (j != 0) {
		memcpy (data.chars, laddr, j);
		ptrace(PTRACE_POKEDATA, pid,
				addr + i * 4, data.val);
	}
}

long getSysCallNo( int pid, struct pt_regs *regs)
{
	long scno = 0;
	ptrace(PTRACE_GETREGSET, pid, NULL, regs);
	scno = ptrace(PTRACE_PEEKTEXT, pid, ( void *)(regs->ARM_pc - 4), NULL);
	if (scno == 0)
		return 0;

	if (scno == 0xef000000) {
		scno = regs->ARM_r7;
	} else {
		if ((scno & 0x0ff00000) != 0x0f900000) {
			return -1;
		}

		/*
		 * Fixup the syscall number
		 */
		scno &= 0x000fffff;
	}
	return scno;

}

void tracePro( int pid)
{
	long scno=0;
	long regV=0;
	struct pt_regs regs;
	char * str;

	scno = getSysCallNo(pid, &regs);
	if (scno == __NR_write)
	{
		str = ( char *) calloc (1, (regs.ARM_r2+1) * sizeof ( char ));
		getdata(pid, regs.ARM_r1, str, regs.ARM_r2);
		reverse(str);
		putdata(pid, regs.ARM_r1, str, regs.ARM_r2);

		printf ( "Reverse str.\n" );

	}
}

int main( int argc, char *argv[])
{  
	if (argc != 2) {
		printf ( "Usage: %s <pid to be traced>\n" , argv[1]);
		return 1;
	}

	pid_t traced_process;
	int status;
	traced_process = atoi (argv[1]);
	if (0 != ptrace(PTRACE_ATTACH, traced_process, NULL, NULL))
	{
		printf ( "Trace process failed:%d.\n" , errno );
		return 1;
	}
	wait(&status);
	printf("status = %d\n", status);
	if (WIFEXITED(status))
	{
		printf("WIFEXITED(status) = %d\n", WIFEXITED(status));
	}
	tracePro(traced_process);
	ptrace(PTRACE_SYSCALL, traced_process, NULL, NULL);

	ptrace(PTRACE_DETACH, traced_process, NULL, NULL);

	return 0;
}
