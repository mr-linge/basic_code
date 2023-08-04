#include "lnkstack.h"
#include <stdlib.h>

int main()
{
	node *top = (node *)malloc(sizeof(node));
	top->info = 333;
	top->next = NULL;

	for (int i = 0; i < 20; i++)
	{
		top = push(top, (i + 100));
	}

	display(top);

	if (empty(top))
	{
		printf("stack is empty!\n");
	}

	datatype retval = read(top);
	printf("retval : %d\n", retval);

	top = pop(top);

	display(top);

	return 0;
}
