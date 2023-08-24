#include <stdio.h>

void test1()
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
}

void test2()
{
    int i = 0;
    do
    {
        printf("%d go here\n", __LINE__);
        i++;
        printf("%d go here\n", __LINE__);
    } while (i != 10);

    printf("i:%d    end\n", i);
}

int main()
{
    test1();
    test2();
    return 0;
}
