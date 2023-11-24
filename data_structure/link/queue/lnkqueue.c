#include "lnkqueue.h"

// init queue
queue *init()
{
    queue *qu;
    qu = (queue *)malloc(sizeof(queue));
    qu->front = NULL;
    qu->rear = NULL;
    return qu;
}

// judge queue empty
int empty(queue qu)
{
    return (qu.front ? 0 : 1);
}

// display all item
void display(queue *qu)
{
    node *p;
    p = qu->front;
    if (!p)
    {
        printf("The link queue is empty!\n");
    }
    while (p)
    {
        printf("%5d ", p->info);
        p = p->next;
    }
    puts("");
}

// read item value on front
datatype read(queue qu)
{
    if (!qu.front)
    {
        printf("The link queue is empty!\n");
        exit(-1);
    }
    return (qu.front->info);
}

// insert
queue *insert(queue *qu, datatype x)
{
    node *p;
    p = (node *)malloc(sizeof(node));
    p->info = x;
    p->next = NULL;
    if (qu->front == NULL) // 当前队列为空，新插入的结点为队列中的唯一结点
    {
        qu->front = qu->rear = p;
    }
    else
    {
        qu->rear->next = p;
        qu->rear = p;
    }
    return qu;
}

// delete item
queue *dele(queue *qu)
{
    node *q;
    if (!qu->front)
    {
        printf("The link queue is empty!\n");
        return qu;
    }
    q = qu->front;
    qu->front = q->next;
    free(q);
    if (qu->front == NULL)
    {
        qu->rear = NULL;
    }
    return qu;
}
