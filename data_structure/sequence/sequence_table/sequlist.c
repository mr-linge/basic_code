#include "sequlist.h"

/*
 * init table
 * */
void init(sequence_list *slt)
{
	slt->size = 0;
}

/*
 * add element from tail
 * */
void append(sequence_list *slt, datatype x)
{
	if (slt->size == MAXSIZE)
	{
		printf("The table is full!\n");
		exit(-1);
	}
	slt->a[slt->size] = x;
	slt->size++;
}

/*
 * traverse the table
 * */
void display(sequence_list slt)
{
	if (slt.size == 0)
	{
		printf("The table is empty!\n");
	}
	else
	{
		for (int i = 0; i < slt.size; i++)
		{
			printf("%5d ", slt.a[i]);
		}
		puts("");
	}
}

/*
 * judge empty
 * */
int empty(sequence_list slt)
{
	return (slt.size == 0 ? 1 : 0);
}

/*
 * find item index by item value
 * */
int find(sequence_list slt, datatype x)
{
	int i = 0;
	while (i < slt.size && slt.a[i] != x)
	{
		i++;
	}
	return (i < slt.size ? i : -1);
}

/*
 * get value by index
 * */
datatype get(sequence_list slt, int i)
{
	if (i < 0 || i >= slt.size)
	{
		printf("The node(%d) is not exist.\n", i);
		exit(-1);
	}
	else
	{
		return slt.a[i];
	}
}

/*
 * insert value to table
 * */
void insert(sequence_list *slt, datatype x, int position)
{
	if (slt->size == MAXSIZE)
	{
		printf("The table is full!\n");
		exit(-1);
	}
	if (position < 0 || position > slt->size)
	{
		printf("The node(%d) is not exist!\n", position);
		exit(-1);
	}
	for (int i = slt->size; i > position; i--)
	{
		slt->a[i] = slt->a[i - 1];
	}

	slt->a[position] = x;
	slt->size++;
}

/*
 * delete item by index
 * */
void delete(sequence_list *slt, int position)
{
	if (slt->size == 0)
	{
		printf("The table is empty!\n");
		exit(-1);
	}
	if (position < 0 || position > slt->size)
	{
		printf("The node(%d) is not exist!\n", position);
		exit(-1);
	}
	for (int i = position; i < slt->size; i++)
	{
		slt->a[i] = slt->a[i + 1];
	}
	slt->size--;
}
