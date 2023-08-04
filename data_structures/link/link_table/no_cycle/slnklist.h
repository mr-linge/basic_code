#include <stdio.h>
#include <stdlib.h>

typedef int datatype;

typedef struct link_node {
	datatype info;
	struct link_node * next;
}node;


// init link list
node *init();
// display all node
void display(node *head);
// find node by index
node * find(node *head, int i);
// insert
node * insert(node *head, datatype x, int i);
// delete node by value
node * delete(node *head, datatype x);
