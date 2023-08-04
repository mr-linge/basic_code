#include <stdio.h>
#include <stdlib.h>

typedef int datatype;

typedef struct dlink_node {
	datatype info;
	struct dlink_node *llink,*rlink;
}dnode;


// init link
dnode *init();
// display all items
void display(dnode *head);
// find item by index
dnode * find(dnode *head, int i);
// insert node
dnode * insert(dnode *head, datatype x, int i);
// delete node
dnode * delete(dnode *head, datatype x);
