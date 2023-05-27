#include <stdio.h>

int main()
{
    int i = 0;
    while (i < 100)
    {
        printf("%d go here\n", __LINE__);
        if (i == 10)
        {
            break;
        }
        i++;
        printf("%d go here\n", __LINE__);
    }
    printf("i:%d    end\n", i);
    return 0;
}
