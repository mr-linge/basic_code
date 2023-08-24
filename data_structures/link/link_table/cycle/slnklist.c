#include "slnklist.h"

// init link
node *init()
{
	return NULL;
}

// address of the last node
node *rear(node *head)
{
	node *p;
	if (!head)
	{
		p = NULL;
	}
	else
	{
		p = head;
		while (p->next != head)
		{
			p = p->next;
		}
	}
	return p;
}

// display all item in link
void display(node *head)
{
	node *p;
	if (!head)
	{
		printf("The link is empty!\n");
	}
	else
	{
		printf("%5d ", head->info);
		p = head->next;
		while (p != head)
		{
			printf("%5d ", p->info);
			p = p->next;
		}
		puts("");
	}
}

// find item which value is x
node *find(node *head, datatype x)
{
	node *q;
	if (!head)
	{
		printf("The link is empty!\n");
		return NULL;
	}
	q = head;
	while (q->next != head && q->info != x)
	{
		q = q->next;
	}
	if (q->info == x)
	{
		return q;
	}
	else
	{
		return NULL;
	}
}

// insert node
node *insert(node *head, datatype x, int i)
{
	node *p, *q, *myrear;
	int j;
	p = (node *)malloc(sizeof(node));
	p->info = x;
	if (i < 0)
	{
		printf("The node(%d) is not exist!\n", i);
		free(p);
		return head;
	}
	if (i == 0 && !head)
	{
		p->next = p;
		head = p;
		return head;
	}
	if (i == 0 && head)
	{
		myrear = rear(head);
		p->next = head;
		myrear->next = p;
		return head;
	}
	if (i > 0 && !head)
	{
		printf("The node(%d) is not exist!\n", i);
		free(p);
		return head;
	}
	if (i > 0 && head)
	{
		q = head;
		j = 1;
		while (i != j && q->next != head)
		{
			q = q->next;
			j++;
		}
		if (i != j)
		{
			printf("The node(%d) is not exist!\n", i);
			free(p);
			return head;
		}
		else
		{
			p->next = q->next;
			q->next = p;
			return head;
		}
	}
}

// delete item by value
node *delete(node *head, datatype x)
{
	node *pre = NULL;
	node *q;
	if (!head)
	{
		printf("The link is empty!\n");
		return head;
	}
	q = head;
	while (q->next != head && q->info != x)
	{ // 没有找遍并且没有找到
		pre = q;
		q = q->next;
	}
	if (q->info != x)
	{ // 没找到
		printf("The node(value = %d) is not exist!\n", x);
	}
	else
	{ // 找到了 进行删除
		if (q != head)
		{
			pre->next = q->next;
			free(q);
		}
		else
		{
			if (head->next == head)
			{
				free(q);
				head = NULL;
			}
			else
			{
				pre = head->next;
				while (pre->next != q)
				{
					pre = pre->next;
				}
				head = head->next;
				pre->next = head;
				free(q);
			}
		}
	}
	return head;
}
