#include "breakpoint.h"

void breakpoint_set(unsigned long vaddr)
{
    getdata(vaddr, 4, backup_machine_code);
    putdata(vaddr, 4, breakpoint);
}

void breakpoint_delete(unsigned long vaddr)
{
    putdata(vaddr, 4, backup_machine_code);
}

// 读取 rn 寄存器
void read_register(unsigned long index)
{
    // printf("%s->%d\n", __FUNCTION__, __LINE__);
    struct pt_regs *regs = (struct pt_regs *)malloc(sizeof(struct pt_regs));
    get_registers(regs);
    // for (size_t i = 0; i < 34; i++)
    // {
    //     printf("x%ld:%llx\n", i, regs->uregs[i]);
    // }
    printf("x%lu:%llx\n", index, regs->uregs[index]);
}

// 写入 rn 寄存器
void write_register(unsigned long index, unsigned long data)
{
    printf("%s->%d\n", __FUNCTION__, __LINE__);
    struct pt_regs *regs = (struct pt_regs *)malloc(sizeof(struct pt_regs));
    get_registers(regs);
    regs->uregs[index] = data;
    set_registers(regs);
    // printf("x%d:%lx\n", index, regs->uregs[index]);
}

// 读取内存
void read_memory(unsigned long vaddr, unsigned long len)
{
    printf("%s->%d\n", __FUNCTION__, __LINE__);
    unsigned char *tmp_data = (unsigned char *)malloc(len);
    memset(tmp_data, '\0', len);

    getdata(vaddr, len, tmp_data);
    for (unsigned long i = 0; i < len; i++)
    {
        printf("%x ", (int)tmp_data[i]);
        if ((i + 1) % 0x10 == 0)
        {
            puts("");
        }
    }
    puts("");
}

// 写入内存
void write_memory(unsigned long vaddr, unsigned long len, char *data)
{
    printf("%s->%d\n", __FUNCTION__, __LINE__);
    unsigned char *tmp_data = (unsigned char *)malloc(len);
    memset(tmp_data, '\0', len);
    memcpy(tmp_data, data, len);

    putdata(vaddr, len, tmp_data);
}

// 从当前断点运行到下个断点
void next_breakpoint()
{
    // printf("%s:%d\n", __FUNCTION__, __LINE__);
    struct pt_regs *regs = (struct pt_regs *)malloc(sizeof(struct pt_regs));
    get_registers(regs);
    unsigned long bp_vaddr = regs->uregs[32];
    // printf("bp_vaddr:0x%lx\n", bp_vaddr);

    breakpoint_delete(bp_vaddr); // 删断点
    ptrace_single_step(pid);     // 单步前进一步
    wait_child_signal(SIGTRAP);  // 单步前进后会自动在下一步设断点,这里应该有个 wait

    breakpoint_set(bp_vaddr); // 重新把原来的断点补回去

    ptrace_cont(pid); // 程序继续执行
}

// 执行 1 条指令
void step_single()
{
    printf("%s->%d\n", __FUNCTION__, __LINE__);
    struct pt_regs *regs = (struct pt_regs *)malloc(sizeof(struct pt_regs));
    get_registers(regs);
    unsigned long current_pc = regs->uregs[32];
    printf("pc:0x%lx\n", current_pc);

    breakpoint_delete(current_pc);
    ptrace_single_step(pid);

    wait_child_signal(SIGTRAP);
}