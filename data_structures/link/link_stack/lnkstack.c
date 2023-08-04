#include "lnkstack.h"

// init stack
node * init() {
	return NULL;
}

//judge stack is empty
int empty(node *top) {
	return (top ? 0 :1);
}

// read item on top
datatype read(node *top) {
	if(!top) {
		printf("link stack is empty!\n");
		exit(-1);
	}
	return top->info;
}

// display all item in link stack
void display(node *top) {
	node *p;
	p = top;
	if(!p) {
		printf("link stack is empty!\n");
	}
	while(p) {
		printf("%5d ", p->info);
		p = p->next;
	}
	puts("");
}

// push
node * push(node *top, datatype x) {
	node *p;
	p = (node *) malloc(sizeof(node));
	p->info = x;
	p->next = top;
	top = p;
	return top;
}

// pop
node * pop(node *top) {
	node *q;
	if(!top) {
		printf("link stack is empty!\n");
		return NULL;
	}
	q = top;
	top = top->next;
	free(q);
	return top;	
}
