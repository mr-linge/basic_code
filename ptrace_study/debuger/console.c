#include "console.h"

// enum debug_model
// {
//     ATTACH,
//     SPAWN
// };

// struct option longopts[] = {
//     {"attach", no_argument, NULL, 0x100},
//     {"spawn", no_argument, NULL, 0x101},
//     {"pid", required_argument, NULL, 0x102},
//     {0, 0, 0, 0},
// };

static int pid;

// void debug_process(enum debug_model model)
// {
//     printf("debug pid:%d\n", pid);
//     ptrace_attach(pid);
// }

// int recv_args_terminal(int argc, char *argv[])
// {
//     // printf("%s:%d\n", __FILE__, __LINE__);
//     enum debug_model model;
//     int c;
//     while ((c = getopt_long(argc, argv, ":", longopts, NULL)) != -1)
//     {
//         // printf("c:%d\n", c);
//         switch (c)
//         {
//         case 0x100:
//         {
//             // printf("%s:%d\n", __FILE__, __LINE__);
//             model = ATTACH;
//         }
//         break;
//         case 0x101:
//         {
//             // printf("%s:%d\n", __FILE__, __LINE__);
//             model = SPAWN;
//         }
//         break;
//         case 0x102:
//         {
//             // printf("%s:%d\n", __FILE__, __LINE__);
//             printf("optarg:%s\n", optarg);
//             pid = atoi(optarg);
//         }
//         break;
//         default:
//             printf("unknow param:%x\n", c);
//             exit(1);
//             break;
//         }
//     }
//     // printf("%s:%d\n", __FILE__, __LINE__);
//     debug_process(model);

//     return pid;
// }

// 读取 rn 寄存器
void read_register(unsigned long index)
{
    // printf("%s->%d\n", __FUNCTION__, __LINE__);
    struct pt_regs *regs = (struct pt_regs *)malloc(sizeof(struct pt_regs));
    get_registers(pid, regs);
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
    get_registers(pid, regs);
    regs->uregs[index] = data;
    set_registers(pid, regs);
    // printf("x%d:%lx\n", index, regs->uregs[index]);
}

// 读取内存
void read_memory(unsigned long vaddr, unsigned long len)
{
    printf("%s->%d\n", __FUNCTION__, __LINE__);
    unsigned char *tmp_data = (unsigned char *)malloc(len);
    memset(tmp_data, '\0', len);

    getdata(pid, vaddr, len, tmp_data);
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

    putdata(pid, vaddr, len, tmp_data);
}

// 程序继续运行
void next_breakpoint()
{
    // printf("%s:%d\n", __FUNCTION__, __LINE__);
    struct pt_regs *regs = (struct pt_regs *)malloc(sizeof(struct pt_regs));
    get_registers(pid, regs);
    unsigned long bp_vaddr = regs->uregs[32];
    // printf("bp_vaddr:0x%lx\n", bp_vaddr);

    breakpoint_delete(pid, bp_vaddr); // 删断点
    ptrace_single_step(pid);          // 单步前进一步
    wait_child_signal(pid, SIGTRAP);  // 单步前进后会自动在下一步设断点,这里应该有个 wait

    breakpoint_set(pid, bp_vaddr); // 重新把原来的断点补回去

    ptrace_cont(pid); // 程序继续执行
}

// 执行 1 条指令
void step_single()
{
    printf("%s->%d\n", __FUNCTION__, __LINE__);
    struct pt_regs *regs = (struct pt_regs *)malloc(sizeof(struct pt_regs));
    get_registers(pid, regs);
    unsigned long current_pc = regs->uregs[32];
    printf("pc:0x%lx\n", current_pc);

    breakpoint_delete(pid, current_pc);
    ptrace_single_step(pid);

    wait_child_signal(pid, SIGTRAP);
}

// 启动控制台
void recv_args_console(int targe_pid)
{
    pid = targe_pid;
    printf("console:");
    unsigned len = 0x100;
    char *cmd = (char *)malloc(len);
    fgets(cmd, len, stdin);
    cmd[strlen(cmd) - 1] = '\0'; // 去掉 '\n'
    // printf("cmd:%s\n",cmd);
    char **p = (char **)calloc(8, 0x100);

    char *p_tmp;
    unsigned k = 0;
    do
    {
        p_tmp = strsep(&cmd, " ");
        if (p_tmp != NULL)
        {
            *(p + k) = p_tmp;
            k++;
        }
    } while (p_tmp != NULL);

    // for (size_t j = 0; j < k; j++)
    // {
    //     printf("--> %s\n", p[j]);
    // }
    if (!strcmp(p[0], "breakpoint"))
    {
        if (!strcmp(p[1], "set"))
        {
            unsigned long vaddr = strtoul(p[2], NULL, 16);
            // printf("vaddr:0x%lx\n", vaddr);
            breakpoint_set(pid, vaddr);
        }
        else if (!strcmp(p[1], "delete"))
        {
            unsigned long vaddr = strtoul(p[2], NULL, 16);
            // printf("vaddr:0x%lx\n", vaddr);
            breakpoint_delete(pid, vaddr);
        }
    }
    else if (k > 0 && !strcmp(p[0], "register"))
    {
        // printf("%d\n", __LINE__);
        // printf("-->%s<--\n", p[1]);
        if (!strcmp(p[1], "read"))
        {
            // printf("%d\n", __LINE__);
            // 读寄存器
            char *tmp = p[2];
            unsigned long r = strtoul((tmp + 1), NULL, 10);
            // printf("register read %ld\n", r);
            // 调用实现的函数
            read_register(r);
        }
        else if (!strcmp(p[1], "write"))
        {
            // printf("%d\n", __LINE__);
            // 写寄存器
            char *tmp = p[2];
            unsigned long r = strtoul((tmp + 1), NULL, 10);
            // printf("register write %ld\n", r);

            tmp = p[3];
            unsigned long data = strtoul(tmp, NULL, 16);
            // printf("data:0x%lx\n", data);

            // 调用实现的函数
            write_register(r, data);
        }
        else
        {
            // printf("%d\n", __LINE__);
            printf("%s 无效指令\n", p[1]);
        }
    }
    else if (!strcmp(p[0], "memory"))
    {
        // printf("%d\n", __LINE__);
        if (!strcmp(p[1], "read"))
        {
            // printf("%d\n", __LINE__);
            // puts("memory read");
            // 内存起始地址
            char *tmp = p[2];
            unsigned long vaddr = strtoul(tmp, NULL, 16);

            tmp = p[3];
            unsigned long len = strtoul(tmp, NULL, 16);
            // printf("len:0x%lx\n", len);

            read_memory(vaddr, len);
        }
        else if (!strcmp(p[1], "write"))
        {
            // printf("%d\n", __LINE__);
            // puts("memory write");
            char *tmp = p[2];
            unsigned long vaddr = strtoul(tmp, NULL, 16);

            tmp = p[3];
            unsigned long len = strtoul(tmp, NULL, 16);
            // printf("len:0x%lx\n", len);

            char *data = p[4];

            write_memory(vaddr, len, data);
        }
        else
        {
            // printf("%d\n", __LINE__);
            printf("%s 无效指令\n", p[1]);
        }
    }
    else if (!strcmp(p[0], "continue"))
    {
        // 继续执行
        ptrace_cont(pid);
    }
    else if (!strcmp(p[0], "next"))
    {
        // 从当前断点出发运行到下一个断点
        next_breakpoint();
        wait_child_signal(pid, SIGTRAP);
    }
    else if (!strcmp(p[0], "step"))
    {
        step_single()
    }
    else if (!strcmp(p[0], "quit"))
    {
        // 退出调试
        ptrace_attach(pid);
    }
    else
    {
        printf("%s 无效指令\n", p[0]);
    }

    free(p);
    free(cmd);
}
