#include "arm64_arch.h"

//  附加到正在运行的进程
int attach_process(pid_t target_pid) {
	printf("+ Tracing process %d\n", target_pid);
	if ((ptrace(PTRACE_ATTACH, target_pid, NULL, NULL)) < 0) {
		perror("ptrace(ATTACH)");
		return -1;
	}
	printf("+ Waiting for process...\n");
	return 0;
}

// 获取寄存器
int get_registers(pid_t target_pid, struct pt_regs *regs) {
	printf("+ Getting Registers\n");
	if ((ptrace(PTRACE_GETREGSET, target_pid, NULL, regs)) != -1) {
		perror("ptrace(GETREGSET)");
		return -1;
	}
	return 0;
}

//  恢复寄存器
int set_registers(pid_t target_pid, struct pt_regs *regs) {
	printf("+ Setting instruction pointer to %p\n", (void *) regs->ARM_pc);
	if ((ptrace(PTRACE_SETREGSET, target_pid, NULL, regs)) != -1) {
		perror("ptrace(SETREGSET)");
		return -1;
	}
	printf("+ Run it!\n");
	return 0;
}

// 让子进程继续运行
int continue_process(pid_t target_pid) {
	int ret = ptrace(PTRACE_CONT, target_pid, NULL, NULL);
	// printf("ret = %d\n", ret);
	if (ret > 0) {
		perror("ptrace(PTRACE_CONT)");
		return -1;
	}
	return 0;
}

//  结束对目标进程的跟踪
int end_tracke_process(pid_t target_pid) {
	if ((ptrace(PTRACE_DETACH, target_pid, NULL, NULL)) != -1) {
		perror("ptrace(DETACH)");
		return -1;
	}
	return 0;
}


// 读取内存数据
void getdata(pid_t target_pid, unsigned long addr, uint8_t *dst, unsigned long len) {
        union {
                long val;
                uint8_t bytes[LONGSIZE];
        } data;
        unsigned long i = 0;
        unsigned long j = len / LONGSIZE;
        uint8_t *laddr = dst;
        while (i < j) {
                data.val = ptrace(PTRACE_PEEKDATA, target_pid, addr + (i * LONGSIZE), NULL);
                memcpy(laddr, data.bytes, LONGSIZE);
                ++i;
                laddr += LONGSIZE;
        }

        unsigned long remainder = len % LONGSIZE;
        if (remainder != 0) { // save the remainder, which less than LONGSIZE
                data.val = ptrace(PTRACE_PEEKDATA, target_pid, addr + (i * LONGSIZE), NULL);
                memcpy(laddr, data.bytes, remainder);
        }
}

// 往内存中写入数据
void putdata(pid_t target_pid, unsigned long addr, uint8_t *src, unsigned long len) {
        union {
                long val;
                uint8_t bytes[LONGSIZE];
        } data;
        unsigned long i = 0;
        unsigned long j = len / LONGSIZE;
        uint8_t *laddr = src;
        while (i < j) {
                memcpy(data.bytes, laddr, LONGSIZE);
                ptrace(PTRACE_POKEDATA, target_pid, addr + (i * LONGSIZE), data.val);
                ++i;
                laddr += LONGSIZE;
        }

        unsigned long remainder = len % LONGSIZE;
        if (remainder != 0) {
                data.val = ptrace(PTRACE_PEEKDATA, target_pid, addr + (i * LONGSIZE), NULL);
                memcpy(data.bytes, laddr, remainder);
                ptrace(PTRACE_POKEDATA, target_pid, addr + (i * LONGSIZE), data.val);
        }
}
