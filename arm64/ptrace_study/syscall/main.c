#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/syscall.h>

// 自义 保存寄存器的 struct, 重命名这些寄存器内容便于理解
struct pt_regs {
	long long uregs[18];
};
#define ARM_cpsr uregs[16]
#define ARM_pc   uregs[15]
#define ARM_lr   uregs[14]
#define ARM_sp   uregs[13]
#define ARM_ip   uregs[12]
#define ARM_fp   uregs[11]
#define ARM_r10  uregs[10]
#define ARM_r9   uregs[9]
#define ARM_r8   uregs[8]
#define ARM_r7   uregs[7]
#define ARM_r6   uregs[6]
#define ARM_r5   uregs[5]
#define ARM_r4   uregs[4]
#define ARM_r3   uregs[3]
#define ARM_r2   uregs[2]
#define ARM_r1   uregs[1]
#define ARM_r0   uregs[0]
#define ARM_ORIG_r0 uregs[17]

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
