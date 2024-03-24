#include "console.h"

// 启动控制台
void recv_args_console()
{
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
            breakpoint_set(vaddr);
        }
        else if (!strcmp(p[1], "delete"))
        {
            unsigned long vaddr = strtoul(p[2], NULL, 16);
            // printf("vaddr:0x%lx\n", vaddr);
            breakpoint_delete(vaddr);
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
        wait_child_signal(SIGTRAP);
    }
    else if (!strcmp(p[0], "step"))
    {
        step_single();
    }
    else if (!strcmp(p[0], "quit"))
    {
        // 退出调试
        ptrace_attach();
    }
    else
    {
        printf("%s 无效指令\n", p[0]);
    }

    free(p);
    free(cmd);
}
