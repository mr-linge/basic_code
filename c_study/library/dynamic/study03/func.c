#include "func.h"

int funcX(int a, int b)
{
    int tmp = add(a, b);
    return tmp - b;
}