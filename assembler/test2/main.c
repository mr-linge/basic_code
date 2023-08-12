#include <stdio.h>

unsigned long add(unsigned long a, unsigned long b) {
    return a + b;
}

int main(void)
{
    printf("%ld\n", add(1, 2));
    return 0;
}
