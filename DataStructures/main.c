#include "sequlist.h"

int main()
{
	sequence_list list;
	init(&list);

	for (int i = 0; i < 30; i++)
	{
		append(&list, (i + 100));
	}

	display(list);

	if (empty(list))
	{
		printf("list is empyt!\n");
	}
	else
	{
		printf("list is not empyt!\n");
	}

	int target_item = 112;
	int retval = find(list, target_item);
	printf("<%d> index is %d\n", target_item, retval);

	int target_index = 12;
	datatype retval2 = get(list, target_index);
	printf("<%d> value:%d\n", target_index, retval2);

	datatype insert_value = 666;
	insert(&list, insert_value, 2);
	display(list);

	delete (&list, 1);
	display(list);

	return 0;
}
