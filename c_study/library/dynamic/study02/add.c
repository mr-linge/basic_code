#include "add.h"

void __attribute__((constructor)) my_init(void)
{
    printf("%s:load library\n", __FUNCTION__);
}

void __attribute__((destructor)) my_fini(void)
{
    printf("%s:unload library\n", __FUNCTION__);
}

int add(int a, int b)
{
    return a + b;
}
