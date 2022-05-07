#include <stdio.h>
#include <stdlib.h>

/*
 * sequence table 顺序表
 * */

#define MAXSIZE 0x100
typedef int datatype;

typedef struct
{
	datatype a[MAXSIZE];
	int size;
} sequence_list;


/*
 * init table
 * */
void init(sequence_list *slt);
/*
 * add element
 * */
void append(sequence_list *slt, datatype x);
/*
 * traverse the table
 * */
void display(sequence_list slt);
/*
 * judge empty
 * */
int empty(sequence_list slt);
/*
 * find item index by value
 * */
int find(sequence_list slt, datatype x);
/*
 * get value by index
 * */
datatype get(sequence_list slt, int i);
/*
 * insert value to table
 * */
void insert(sequence_list *slt, datatype x, int position);
/*
 * delete item by index
 * */
void delete(sequence_list *slt, int position);
