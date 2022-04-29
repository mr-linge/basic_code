#include "hook_util.h"

pid_t pid;

int onEnter(char *func_name, unsigned long func_vaddr, long long *argv)
{
    printf("onEnter -------> func_name: %s, func_vaddr: %lx <-------\n", func_name, func_vaddr);
    if (strcmp(func_name, "func20") == 0)
    {
        for (int k = 0; k < 20; k++)
        {
            printf("argv[%d] = 0x%llx\n", k, argv[k]);
        }
        argv[1] = 0xaaaaaaaa;
    }

    if (strcmp(func_name, "func21") == 0)
    {
        for (int k = 0; k < 20; k++)
        {
            printf("argv[%d] = 0x%llx\n", k, argv[k]);
        }
        argv[1] = 0xbbbbbbbb;
    }

    if (strcmp(func_name, "func3") == 0)
    {
        printf("return value : 0x%llx\n", argv[0]);
        uint8_t str[0x100];
        getdata(pid, argv[0], str, 20);
        printf("argv[0]:%s\n", (char *)argv[0]);
        // argv[1] = 0xaaaaaaaa;
    }

    return 0;
}

int onLeave(char *func_name, unsigned long func_vaddr, long long *argv)
{
    printf("onLeave -------> func_name: %s, func_vaddr: %lx <-------\n", func_name, func_vaddr);
    if (strcmp(func_name, "func20") == 0)
    {
        printf("return value : 0x%llx\n", *argv);
        *argv = 0xfc20;
    }

    if (strcmp(func_name, "func21") == 0)
    {
        printf("return value : 0x%llx\n", *argv);
        *argv = 0xfc21;
    }

    if (strcmp(func_name, "func3") == 0)
    {
        printf("return value : 0x%llx\n", *argv);
        uint8_t str[0x100];
        getdata(pid, *argv, str, 20);
        printf("*argv:%s\n", (char *)*argv);
        // *argv = 0xaaaaaaaa;
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
        return -1;
    }
    signal(SIGINT, sighandler);

    pid = atoi(argv[1]);

    int func_count = 3;
    struct Hooked_func funcs[func_count];

    struct Hooked_func func20;
    memset(&func20,0,sizeof(struct Hooked_func));
    char *func20_name = "func20";
    memcpy(func20.func_name, func20_name, strlen(func20_name));
    char *func20_module_path = "/data/local/tmp/work/tracee";
    memcpy(func20.module_path, func20_module_path, strlen(func20_module_path));
    func20.func_vaddr = 0; // 0x5a5726ba00;
    func20.num_params = 20;

    struct Hooked_func func21;
    memset(&func21,0,sizeof(struct Hooked_func));
    char *func21_name = "func21";
    memcpy(func21.func_name, func21_name, strlen(func21_name));
    char *func21_module_path = "/data/local/tmp/work/tracee";
    memcpy(func21.module_path, func21_module_path, strlen(func21_module_path));
    func21.func_vaddr = 0; // 0x5a5726bba4;
    func21.num_params = 20;

    struct Hooked_func func3;
    memset(&func3,0,sizeof(struct Hooked_func));
    char *func3_name = "func3";
    memcpy(func3.func_name, func3_name, strlen(func3_name));
    char *func3_module_path = "/data/local/tmp/work/tracee";
    memcpy(func3.module_path, func3_module_path, strlen(func3_module_path));
    func3.func_vaddr = 0; // 0x5a5726bba4;
    func3.num_params = 1;

    funcs[0] = func20;
    funcs[1] = func21;
    funcs[2] = func3;

    hook(pid, funcs, func_count, (unsigned long)&onEnter, (unsigned long)&onLeave);

    return 0;
}