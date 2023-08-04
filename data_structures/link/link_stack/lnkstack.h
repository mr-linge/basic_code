#include <stdio.h>
#include <stdlib.h>

typedef int datatype;

typedef struct link_node {
	datatype info;
	struct link_node * next;
}node;


// init stack
node * init();
//judge stack is empty
int empty(node *top);
// read item on top
datatype read(node *top);
// display all item in link stack
void display(node *top);
// push
node * push(node *top, datatype x);
// pop
node * pop(node *top);
