#include "hook_util.h"

int onEnter(char *func_name, unsigned long func_vaddr, long long *argv)
{
    printf("func_name: %s, func_vaddr: %lx\n", func_name, func_vaddr);
    for (int k = 0; k < 20; k++)
    {
        printf("argv[%d] = 0x%llx\n", k, argv[k]);
    }
    if (strcmp(func_name, "func20") == 0)
    {
        argv[1] = 0xaaaaaaaa;
    }

    if (strcmp(func_name, "func21") == 0)
    {
        argv[1] = 0xbbbbbbbb;
    }

    return 0;
}

int onLeave(char *func_name, unsigned long func_vaddr, long long *argv)
{
    printf("func_name: %s, func_vaddr: %lx\n", func_name, func_vaddr);
    printf("return value : 0x%llx\n", *argv);
    if (strcmp(func_name, "func20") == 0)
    {
        *argv = 0xfc20;
    }

    if (strcmp(func_name, "func21") == 0)
    {
        *argv = 0xfc21;
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

    pid_t pid = atoi(argv[1]);
    signal(SIGINT, sighandler);

    int func_count = 2;
    struct Hooked_func funcs[func_count];

    struct Hooked_func func20;
    char *func20_name = "func20";
    memcpy(func20.func_name, func20_name, strlen(func20_name));
    char *func20_module_path = "/data/local/tmp/work/tracee";
    memcpy(func20.module_path, func20_module_path, strlen(func20_module_path));
    func20.func_vaddr = 0;//0x5a5726ba00;
    func20.num_params = 20;

    struct Hooked_func func21;
    char *func21_name = "func21";
    memcpy(func21.func_name, func21_name, strlen(func21_name));
    char *func21_module_path = "/data/local/tmp/work/tracee";
    memcpy(func21.module_path, func21_module_path, strlen(func21_module_path));
    func21.func_vaddr = 0;//0x5a5726bba4;
    func21.num_params = 20;

    funcs[0] = func20;
    funcs[1] = func21;

    hook(pid, funcs, func_count, (unsigned long)&onEnter, (unsigned long)&onLeave);

    return 0;
}