#include <sys/ptrace.h>  // ptrace
#include <unistd.h>      // pid_t
#include <stdint.h>
#include <string.h>
#include <stdio.h>

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

#define LONGSIZE  sizeof(long)


//  附加到正在运行的进程
int attach_process(pid_t target_pid);

// 获取寄存器
int get_registers(pid_t target_pid, struct pt_regs *regs);

//  恢复寄存器
int set_registers(pid_t target_pid, struct pt_regs *regs);

// 让子进程继续运行
int continue_process(pid_t target_pid);

//  结束对目标进程的跟踪
int end_tracke_process(pid_t target_pid);


// 读取内存数据
void getdata(pid_t target_pid, unsigned long addr, uint8_t *dst, unsigned long len);
// 往内存中写入数据
void putdata(pid_t target_pid, unsigned long addr, uint8_t *src, unsigned long len);
