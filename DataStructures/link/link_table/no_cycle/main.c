#include "slnklist.h"

int main()
{
	node *link = init();

	for (int i = 0; i < 20; i++)
	{
		insert(link, (100 + i), i);
	}

	display(link);

	node *l0 = find(link, 0);
	printf("link 0, value:%d\n", l0->info);

	node *l2 = find(link, 2);
	printf("link 2, value:%d\n", l2->info);

	insert(link, 666, 2);

	display(link);

	insert(link, 888, 0);

	display(link);

	return 0;
}
