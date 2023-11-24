#include "seqstack.h"

/*
 * init stack
 * */
void init(sequence_stack *st)
{
	st->top = 0;
}

/*
 * judge stack empty
 * */
int empty(sequence_stack st)
{
	return (st.top ? 0 : 1);
}

/*
 * read data from stack top
 * */
datatype read(sequence_stack st)
{
	if (empty(st))
	{
		printf("stack is empty\n");
		exit(-1);
	}
	else
	{
		return st.a[st.top - 1];
	}
}

/*
 * push
 * */
void push(sequence_stack *st, datatype x)
{
	if (st->top == MAXSIZE)
	{
		printf("stack is full\n");
		exit(-1);
	}
	else
	{
		st->a[st->top] = x;
		st->top++;
	}
}

/*
 * pop
 * */
datatype pop(sequence_stack *st)
{
	if (st->top == 0)
	{
		printf("The stack is empty\n");
		exit(-1);
	}
	else
	{
		st->top--;
		datatype retval = st->a[st->top];
		return retval;
	}
}
