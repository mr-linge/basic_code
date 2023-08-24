#include "dlinklist.h"

// init link
dnode *init()
{
	return NULL;
}

// display all items
void display(dnode *head)
{
	dnode *p;
	p = head;
	if (!p)
	{
		printf("The link is empty!\n");
	}
	else
	{
		while (p)
		{
			printf("%5d ", p->info);
			p = p->rlink;
		}
		puts("");
	}
}

// find item by index
dnode *find(dnode *head, int i)
{
	int j = 1;
	dnode *p = head;
	if (i < 1)
	{
		printf("The node(%d) is not exist!\n", i);
		return NULL;
	}
	while (p && i != j)
	{
		p = p->rlink;
		j++;
	}
	if (!p)
	{
		printf("The node(%d) is not exist!\n", i);
		return NULL;
	}
	return p;
}

// insert node
dnode *insert(dnode *head, datatype x, int i)
{
	dnode *p, *q;
	p = (dnode *)malloc(sizeof(dnode));
	p->info = x;
	if (i == 0)
	{ // 在最前面插入新节点
		p->llink = NULL;
		p->rlink = head;
		if (!head)
		{
			head = p;
		}
		else
		{
			head->llink = p;
			head = p;
		}
		return p;
	}
	q = find(head, i);
	if (!q)
	{
		printf("The node(%d) is not exist!\n", i);
		free(p);
		return head;
	}
	if (q->rlink == NULL)
	{ // 在最后面插入节点
		p->rlink = q->rlink;
		p->llink = q;
		q->rlink = p;
	}
	else
	{
		p->rlink = q->rlink;
		p->llink = q;
		q->rlink->llink = p;
		q->rlink = p;
	}
	return head;
}

// delete node
dnode *delete(dnode *head, datatype x)
{
	dnode *q;
	if (!head)
	{
		printf("The link is empty!\n");
		return head;
	}
	q = head;
	while (q && q->info != x)
	{
		q = q->rlink;
	}
	if (!q)
	{
		printf("The node(value :%d) is not exist!\n", x);
	}
	if (q == head && head->rlink)
	{ // 被删除的结点是第一个结点，并且表中不止一个结点
		head = head->rlink;
		head->llink = NULL;
		free(q);
		return head;
	}
	if (q == head && !head->rlink)
	{ // 被删除的结点是第一个结点，并且表中只有一个结点
		free(q);
		return NULL;
	}
	else
	{
		if (!q->rlink)
		{ // 被删除的结点是最后一个结点
			q->llink->rlink = NULL;
			free(q);
			return head;
		}
		else
		{
			q->llink->rlink = q->rlink;
			q->rlink->llink = q->llink;
			free(q);
			return head;
		}
	}
}
