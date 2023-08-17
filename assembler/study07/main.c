#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

int c_test_func(int num);

int new2_c_test_func(int i)
{
    return i * 100;
}

void inject_imitate()
{
    char *inject_dylib_path = "./libhook.so";
    if (access(inject_dylib_path, F_OK) == 0)
    {
        // 打开动态链接库
        void *handle = dlopen(inject_dylib_path, RTLD_NOW);
        // printf("handle = %p\n", handle);
        if (!handle)
        {
            fprintf(stderr, "%s\n", dlerror());
        }
    }
    else
    {
        printf("%s is not exist\n", inject_dylib_path);
    }
}

void create_space() {
    
}

int main(int argc, char const *argv[])
{
    inject_imitate();

    getchar();
    printf("c_test_func vaddr:0x%lx\n", (unsigned long)&c_test_func);
    int ret = c_test_func(10);
    printf("c_test_func ret:%d\n", ret);

    getchar();
    return 0;
}
