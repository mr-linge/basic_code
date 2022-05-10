#include "slnklist.h"

int main() {
	node *link = (node *) malloc(sizeof(node));
	link->info = 999;
	link->next = link;

	// node *link = NULL;

	for (int i = 0; i < 20; i++)
	{
		link = insert(link, (100 + i), i + 1);
	}

	display(link);

	node *l0 = find(link, 101);
	printf("value:%d\n", l0->info);

	node *l2 = find(link, 102);
	printf("value:%d\n", l2->info);

	link = insert(link, 666, 1);

	display(link);

	link = insert(link, 888, 10);

	display(link);

	link = delete(link,888);

	display(link);

	return 0;
}
