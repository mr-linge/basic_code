#include <stdio.h>

#define PI 3.14
#define average(a, b) (a + b) / 2

int main()
{
    float result = PI * 2;
    printf("result = %f\n", result);
    int max = average(12, 43);
    printf("max = %d\n", max);
    return 0;
}
