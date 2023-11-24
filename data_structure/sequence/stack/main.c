#include "seqstack.h"

int main() {
	sequence_stack stack;
	init(&stack);
	if(empty(stack)) {
		puts("stack is empty!");
	}

	for(int i = 0; i < 20; i++) {
		push(&stack,(i + 100));
	}

	datatype value = read(stack);
	printf("top value:%d\n", value);

	datatype value2 = pop(&stack);
	printf("pop value:%d\n", value2);

	return 0;
}
