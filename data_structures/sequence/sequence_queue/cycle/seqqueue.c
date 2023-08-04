#include "seqqueue.h"

// init queue
void init(sequence_queue *sq) {
	sq->front = sq->rear = 0;
}

// judge queue empty
int empty(sequence_queue sq) {
	return (sq.front == sq.rear ? 1 : 0);
}

// display queue
void display(sequence_queue sq) {
	if(empty(sq)){
		puts("queue is empty!\n");
		exit(-1);
	}else {
		for(int i = sq.front; i < sq.rear; i++) {
			printf("%5d ", sq.a[i]);
		}
		puts("");
	}
}

// get value in queue front
datatype get(sequence_queue sq) {
	if(empty(sq)) {
		puts("queue is empty!\n");
		exit(-1);
	}
	return sq.a[sq.front];
}

// insert value
void insert(sequence_queue *sq, datatype x) {
	if((sq->rear + 1) % MAXSIZE == sq->front) {
		printf("queue is full\n");
		exit(-1);
	}
	sq->a[sq->rear] = x;
	sq->rear = (sq->rear + 1) % MAXSIZE;
}

// delete item
void delete(sequence_queue *sq) {
	if(sq->front == sq->rear) {
		puts("queue is empty!\n");
		exit(-1);
	}
	sq->front = (sq->front + 1) % MAXSIZE;
}
