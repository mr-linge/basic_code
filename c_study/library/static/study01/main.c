#include <stdio.h>
#include "add.h"
#include "sub.h"

int main()
{
    printf("1 + 2 = %d\n", add(1, 2));
    printf("3 - 2 = %d\n", sub(3, 2));
    return 0;
}