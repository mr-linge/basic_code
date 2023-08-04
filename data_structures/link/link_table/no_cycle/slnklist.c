#include "slnklist.h"

// init link list
node *init() {
	node *head;
	head = (node *) malloc(sizeof(node));
	head->next = NULL;
	return head;
}

// display all node
void display(node *head) {
	node *p = head->next;
	if(!p) {
		printf("The link is empty!\n");
	}else {
		while(p) {
			printf("%5d ",p->info);
			p = p->next;
		}
		puts("");
	}
}

// find node by index
node * find(node *head, int i) {
	node *p = head;
	int j = 0;
	if(i < 0) {
		printf("The node(%d) is not exist!\n", i);
		return NULL;
	}else if(i == 0) {
		return p;
	}
	while(p && j != i) {
		p = p->next;
		j++;
	}
	return p;
}

// insert
node * insert(node *head, datatype x, int i) {
	node *p,*q;
	q = find(head, i);
	if(!q) {
		printf("The node(%d) is not exist!\n",i);
		return head;
	}

	p = (node *) malloc(sizeof(node));
	p->info = x;
	p->next = q->next;
	q->next = p;
	return head;
}

// delete node by value
node * delete(node *head, datatype x) {
	node * pre = head;
	node * q = head->next;
	while(q && q->info != x) {
		pre = q;
		q = q->next;
	}
	if(q) {
		pre->next = q->next;
		free(q);
	}
	return head;
}
