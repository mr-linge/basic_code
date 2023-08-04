#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

void test()
{
    int y = 0;
    int x = 1;

    asm volatile("add %w0, %w1, 1"
                 : "=r"(y)
                 : "r"(x)
                 :);
}

int main(int argc, char const *argv[])
{
    test();
    return 0;
}
