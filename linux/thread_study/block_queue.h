#include <pthread.h>

typedef struct list_node
{
    int data;
    struct node *next_node;
} ListNode;

typedef struct list
{
    ListNode *firstNode;
    int size;
    int full_size;
    pthread_mutex_t mutex;
    pthread_cond_t cond_for_producer;
    pthread_cond_t cond_for_customer;
} List;

List *block_queue_create(int full_size);

void block_queue_destroy(List *plist);

void block_queue_push(List *plist, int data);

int block_queue_pop(List *plist, int *p_data);
