#include "hook.h"

// 函数指针用于保留原来的执行流程
int (*old_c_test_func)(int i);

// 新函数
int new_c_test_func(int i)
{
    int origin_ret = old_c_test_func(i);
    printf("原来的参数:%d,返回值:%d\n", i, origin_ret);
    // printf("go new_c_test_func\n");
    return i * 10;
}

void __attribute__((constructor)) dylibInject(void)
{
    char *module_name = "/data/local/tmp/main";
    unsigned long module_vaddr = get_module_vaddr(module_name);
    printf("%s --> %s vaddr:0x%lx\n", __FUNCTION__, module_name, module_vaddr);

    unsigned long target_func_vaddr = module_vaddr + 0x18bc;

    printf("new_c_test_func vaddr:0x%lx\n", (unsigned long)&new_c_test_func);
    hook((void *)target_func_vaddr, (void *)&new_c_test_func, (void **)&old_c_test_func);
}