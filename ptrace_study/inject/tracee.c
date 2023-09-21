#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>

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

int main(int argc, char const *argv[])
{
    // printf("==================== current pid:%d ====================\n", getpid());
    // printf("%s:%d c_test_func vaddr:0x%lx\n", __FILE__, __LINE__, (unsigned long)&c_test_func);
    // log_matchine_code((void *)&c_test_func, 0x30);
    // getchar();
    // int ret = c_test_func(10);
    // printf("c_test_func ret:%d\n", ret);
    // log_matchine_code((void *)&c_test_func, 0x30);
    // getchar();
    char *test_str = "This is a test string";
    char *memory_heap = (char *)malloc(0x100);
    memset(memory_heap, '\0', 0x100);
    memcpy(memory_heap, test_str, strlen(test_str));
    char memory_stack[0x100];
    memcpy(memory_stack, memory_heap, 0x100);

    unsigned long t1_stack_vaddr = (unsigned long)memory_stack - 0x100;
    unsigned long index = 0;
    while (1)
    {
        printf("%lx****************** pid:%d *******************\n", index++, getpid());
        printf("&c_test_func         addr: %p\n", &c_test_func);
        printf("&dlopen              addr: %p\n", &dlopen);
        printf("&printf              addr: %p\n", &printf);
        printf("memory_heap          addr: 0x%lx\n", (unsigned long)memory_heap);
        printf("%s\n", memory_heap);
        printf("&memory_stack        addr: 0x%lx\n", (unsigned long)&memory_stack);
        printf("%s\n", memory_stack);
        log_matchine_code(memory_stack, 0x20);
        printf("t1_stack_vaddr:0x%lx\n",t1_stack_vaddr);
        log_matchine_code((void *)t1_stack_vaddr, 0x20);
        long ret = c_test_func(10);
        printf("ret = 0x%lx\n", ret);
        sleep(10);
        // getchar();
    }
    return 0;
}
