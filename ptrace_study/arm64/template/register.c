#include <stdio.h>       // printf
#include <stdlib.h>
#include <sys/wait.h>

#include "arm64_arch.h"

int main(int argc, char *argv[])
{
        if(argc != 2)
        {
                printf("please input pid...\n");
                return -1;
        }

        pid_t target_pid = atoi(argv[1]);
        if(target_pid == 0) {
                printf("%s is Illegal parameter\n",argv[1]);
                return -1;
        }

        struct pt_regs regs;

        attach_process(target_pid);
        wait(NULL);

        get_registers(target_pid, &regs);
        printf("ARM_r0:0x%llx, ARM_r1:0x%llx, ARM_r2:0x%llx, ARM_r3:0x%llx, ARM_r4:0x%llx, ARM_r5:0x%llx, ARM_r6:0x%llx, ARM_r7:0x%llx, ARM_r8:0x%llx, ARM_r9:0x%llx, ARM_r10:0x%llx, ARM_ip:0x%llx, ARM_sp:0x%llx, ARM_lr:0x%llx, ARM_pc:0x%llx\n",regs.ARM_r0, regs.ARM_r1, regs.ARM_r2, regs.ARM_r3, regs.ARM_r4, regs.ARM_r5, regs.ARM_r6, regs.ARM_r7, regs.ARM_r8, regs.ARM_r9, regs.ARM_r10, regs.ARM_ip, regs.ARM_sp, regs.ARM_lr, regs.ARM_pc);

        continue_process(target_pid);

        set_registers(target_pid, &regs);

        continue_process(target_pid);

        return 0;
}

