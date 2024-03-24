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
    char *test_str = "AAAAAAABBBBB";
    char *memory_heap = (char *)malloc(0x100);
    memset(memory_heap, '\0', 0x100);
    memcpy(memory_heap, test_str, strlen(test_str));

    unsigned long index = 0;
    while (1)
    {
        printf("%lx****************** pid:%d *******************\n", index++, getpid());
        printf("&c_test_func         addr: %p\n", &c_test_func);
        printf("&dlopen              addr: %p\n", &dlopen);
        printf("&printf              addr: %p\n", &printf);
        printf("memory_heap          addr: 0x%lx\n", (unsigned long)memory_heap);
        printf("%s\n", memory_heap);
        long ret = c_test_func(10);
        printf("ret = 0x%lx\n", ret);
        // sleep(5);
        getchar();
    }
    return 0;
}
