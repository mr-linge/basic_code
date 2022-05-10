#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 0x100
typedef int datatype;

typedef struct {
	datatype a[MAXSIZE];
	int front;
	int rear;
}sequence_queue;

// init queue
void init(sequence_queue *sq);
// judge queue empty
int empty(sequence_queue sq);
// display queue
void display(sequence_queue sq);
// get value in queue front
datatype get(sequence_queue sq);
// insert value
void insert(sequence_queue *sq, datatype x);
// delete item
void delete(sequence_queue *sq);