#include "dlinklist.h"

int main() {
	dnode *link = (dnode *) malloc(sizeof(dnode));
	link->rlink = NULL;

	for(int i = 0; i < 20; i++) {
		link = insert(link,(100 + i),i + 1);
	}
	
	display(link);

	dnode *tmp = find(link,2);
	printf("find node value : %d\n", tmp->info);

	delete(link, 104);

	display(link);

	return 0;
}
