#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <dlfcn.h>

char *libpath = "./libtest.so";

void log_matchine_code(void *vaddr, unsigned long len)
{
    unsigned char *machine_codes = (unsigned char *)vaddr;
    for (unsigned long i = 0; i < len; i++)
    {
        printf("%02x ", (int)machine_codes[i]);
        if ((i + 1) % 4 == 0)
        {
            puts("");
        }
    }
}

int c_test_func(int num)
{
    return num + 10;
}

void test_log()
{
    printf("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
}

int main(int argc, char const *argv[])
{
    // printf("%s:%d c_test_func vaddr:0x%lx\n", __FILE__, __LINE__, (unsigned long)&c_test_func);
    // log_matchine_code((void *)&c_test_func, 0x50);
    // getchar();
    // // int ret = c_test_func(10);
    // // printf("c_test_func ret:%d\n", ret);
    // log_matchine_code((void *)&c_test_func, 0x50);
    // getchar();

    while (1)
    {
        printf("****************** pid:%d *******************\n", getpid());
        // printf("&c_test_func         addr: %p\n", &c_test_func);
        printf("&test_log            addr: %p\n", &test_log);
        // printf("&dlopen              addr: %p\n", &dlopen);
        printf("&printf              addr: %p\n", &printf);
        long ret = c_test_func(10);
        printf("ret = 0x%lx\n", ret);
        sleep(5);
        // getchar();
    }

    return 0;
}
