#include <stdio.h>

void __attribute__((constructor)) dylibInject(void)
{
    printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
}
