#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 0x100
typedef int datatype;

typedef struct {
	datatype a[MAXSIZE];
	int top;
}sequence_stack;


/*
 * init stack
 * */
void init(sequence_stack *st);
/*
 * judge stack empty
 * */
int empty(sequence_stack st);
/*
 * read data from stack top
 * */
datatype read(sequence_stack st);
/*
 * push
 * */
void push(sequence_stack *st, datatype x);
/*
 * pop
 * */
datatype pop(sequence_stack *st);