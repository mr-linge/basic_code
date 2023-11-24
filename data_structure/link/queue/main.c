#include "lnkqueue.h"

int main()
{
    queue *qu = init();

    for (int i = 0; i < 20; i++)
    {
        qu = insert(qu, (i + 100));
    }

    display(qu);

    if (empty(*qu))
    {
        printf("The link queue is empty!\n");
    }

    datatype retval = read(*qu);
    printf("retval : %d\n", retval);

    dele(qu);

    display(qu);

    return 0;
}