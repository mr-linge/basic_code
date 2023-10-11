#include <stdio.h>
#include <dlfcn.h>

int main(int argc, char const *argv[])
{
    printf("&dlopen              addr: %p\n", &dlopen);
    return 0;
}