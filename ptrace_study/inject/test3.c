#include <stdio.h>
#include <string.h>
#include "address.h"

// int add(int a, int b);

int main(int argc, char const *argv[])
{
    printf("==================== current pid:%d ====================\n", getpid());
    // int a = 10;
    // int b = 20;
    // printf("add(%d,%d) = %d\n", a, b, add(a, b));
    char *dlopen_module = "libdl.so";
    unsigned long dlopen_module_vaddr = get_module_vaddr(getpid(), dlopen_module);
    printf("%s: dlopen_module_vaddr:0x%lx\n", __FILE__, dlopen_module_vaddr);

    unsigned long dlopen_offset = get_symbol_offset(dlopen_module, "dlopen", 0);
    printf("%s: dlopen_offset:0x%lx\n\n", __FILE__, dlopen_offset);

    char *c_module = "libc.so";
    unsigned long std_c_module_vaddr = get_module_vaddr(getpid(), c_module);
    printf("%s: std_c_module_vaddr:0x%lx\n", __FILE__, std_c_module_vaddr);

    unsigned long mmap_symbol_offset = get_symbol_offset(c_module, "mmap", 0);
    printf("%s: mmap_symbol_offset:0x%lx\n\n", __FILE__, mmap_symbol_offset);

    char *a_module = "/data/local/tmp/liba.so";
    unsigned long a_module_vaddr = get_module_vaddr(getpid(), a_module);
    printf("a_module_vaddr:0x%lx\n", a_module_vaddr);

    unsigned long add_offset = get_symbol_offset(a_module, "add", 1);
    printf("add_offset:0x%lx\n", add_offset);

    getchar();

    return 0;
}
