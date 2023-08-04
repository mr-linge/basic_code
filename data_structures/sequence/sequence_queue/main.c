#include "seqqueue.h"
#include <stdio.h>

int main() {
	sequence_queue queue;
	init(&queue);

	if(empty(queue)) {
		printf("queue is empty!\n");
	}

	for(int i = 0; i < 20; i++) {
		insert(&queue,(i + 100));
	}

	display(queue);

	delete(&queue);
	
	display(queue);

	return 0;
}
