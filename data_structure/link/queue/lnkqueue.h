#include <stdio.h>
#include <stdlib.h>

typedef int datatype;

typedef struct link_node
{
	datatype info;
	struct link_node *next;
} node;

typedef struct
{
	node *front, *rear; // 队首与队尾指针
} queue;

// init queue
queue *init();
// judge queue empty
int empty(queue qu);
// display all item
void display(queue *qu);
// read item value on front
datatype read(queue qu);
// insert
queue *insert(queue *qu, datatype x);
// delete item
queue *dele(queue *qu);