#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

// 读取 rn 寄存器
void read_register(unsigned long index)
{
    printf("%s->%d\n", __FUNCTION__, __LINE__);
}

// 写入 rn 寄存器
void write_register(unsigned long index, unsigned long data)
{
    printf("%s->%d\n", __FUNCTION__, __LINE__);
}

// 读取内存
void read_memory(unsigned long vaddr, unsigned long len)
{
    printf("%s->%d\n", __FUNCTION__, __LINE__);
}

// 写入内存
void write_memory(unsigned long vaddr, unsigned long len, char *data)
{
    printf("%s->%d\n", __FUNCTION__, __LINE__);
}

// 程序继续运行
void continue_process()
{
    printf("%s->%d\n", __FUNCTION__, __LINE__);
}

// 执行 n 条指令
void step_move(unsigned long n)
{
    printf("%s->%d\n", __FUNCTION__, __LINE__);
}

// 设置断点
void breakpoint_set(unsigned long vaddr)
{
    printf("%s->%d\n", __FUNCTION__, __LINE__);
}

// 删除断点
void breakpoint_delete(unsigned long vaddr)
{
    printf("%s->%d\n", __FUNCTION__, __LINE__);
}

void recv_args_cli()
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
            printf("vaddr:0x%lx\n", vaddr);
            breakpoint_set(vaddr);
        }
        else if (!strcmp(p[1], "delete"))
        {
            unsigned long vaddr = strtoul(p[2], NULL, 16);
            printf("vaddr:0x%lx\n", vaddr);
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
            printf("register read %ld\n", r);
            // 调用实现的函数
            read_register(r);
        }
        else if (!strcmp(p[1], "write"))
        {
            // printf("%d\n", __LINE__);
            // 写寄存器
            char *tmp = p[2];
            unsigned long r = strtoul((tmp + 1), NULL, 10);
            printf("register write %ld\n", r);

            tmp = p[3];
            unsigned long data = strtoul(tmp, NULL, 16);
            printf("data:0x%lx\n", data);

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
            printf("len:0x%lx\n", len);

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
            printf("len:0x%lx\n", len);

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
        // 越过断点继续执行
        continue_process();
    }
    else if (!strcmp(p[0], "step"))
    {
        unsigned long len = strtoul(p[1], NULL, 16);
        printf("len:0x%lx\n", len);
        step_move(len);
    }
    else if (!strcmp(p[0], "quit"))
    {
        // 退出调试
    }
    else
    {
        // printf("%d\n", __LINE__);
        printf("%s 无效指令\n", p[0]);
    }
    // printf("%d\n", __LINE__);
    free(p);
    free(cmd);
}

enum debug_model
{
    ATTACH,
    SPAWN
};

struct option longopts[] = {
    {"attach", no_argument, NULL, 0x100},
    {"spawn", no_argument, NULL, 0x101},
    {"pid", required_argument, NULL, 0x102},
    {0, 0, 0, 0},
};

static int pid;

void recv_args_terminal(int argc, char *argv[])
{
    enum debug_model model;
    int c;
    while ((c = getopt_long(argc, argv, ":", longopts, NULL)) != -1)
    {
        printf("c:%d\n", c);
        switch (c)
        {
        case 0x100:
        {
            printf("%s:%d\n", __FILE__, __LINE__);
            model = ATTACH;
        }
        break;
        case 0x101:
        {
            printf("%s:%d\n", __FILE__, __LINE__);
            model = SPAWN;
        }
        break;
        case 0x102:
        {
            printf("%s:%d\n", __FILE__, __LINE__);
            printf("optarg:%s\n", optarg);
            pid = atoi(optarg);
        }
        break;
        default:
            printf("unknow param:%x\n", c);
            break;
        }
    }
    printf("%s:%d\n", __FILE__, __LINE__);
}

int main(int argc, char *argv[])
{
    // while (1)
    // {
    //     recv_args_cli();
    // }
    // char buffer[0x100];
    // fgets(buffer, sizeof(buffer), stdin);
    // puts(buffer);
    recv_args_terminal(argc, argv);
    return 0;
}
