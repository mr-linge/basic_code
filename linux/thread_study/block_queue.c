#include "block_queue.h"
#include <stdlib.h>
#include <stdio.h>

List *block_queue_create(int full_size)
{
    // 分配内存
    List *plist = malloc(sizeof(List));
    plist->full_size = full_size;
    plist->size = 0;
    plist->firstNode = NULL;

    // 初始化锁与条件等待.
    pthread_mutex_init(&(plist->mutex), NULL);
    pthread_cond_init(&(plist->cond_for_customer), NULL);
    pthread_cond_init(&(plist->cond_for_producer), NULL);

    return plist;
}

void block_queue_destroy(List *plist)
{
    int data = 0;
    int ret = 0;

    // 销毁所有数据。
    while (ret == 0)
    {
        if (plist->size > 0)
        {
            ret = block_queue_pop(plist, &data);
        }
        else
        {
            break;
        }
    }

    // 销毁锁与条件等待
    pthread_cond_destroy(&(plist->cond_for_producer));
    pthread_cond_destroy(&(plist->cond_for_customer));
    pthread_mutex_destroy(&(plist->mutex));

    // 释放内存
    free(plist);
}

void block_queue_push(List *plist, int data)
{
    // lock
    pthread_mutex_lock(&(plist->mutex));

    while (plist->size >= plist->full_size)
    {
        printf("队列已满，生产者阻塞。目前仓储数量:%d\n", plist->size);
        pthread_cond_wait(&(plist->cond_for_producer), &(plist->mutex));
    }

    ListNode *curNode = plist->firstNode;
    if (curNode == NULL)
    {
        ListNode *new_node = malloc(sizeof(ListNode));
        new_node->next_node = NULL;
        new_node->data = data;
        plist->firstNode = new_node;
        plist->size++;

        printf("队列刚才生产一条数据，通知阻塞的消费者。目前仓储数量:%d\n", plist->size);
        pthread_cond_signal(&(plist->cond_for_customer));

        // unlock
        pthread_mutex_unlock(&(plist->mutex));
        return;
    }

    ListNode *lastNode = curNode;
    curNode = (ListNode *)curNode->next_node;
    for (;;)
    {
        if (curNode == NULL)
        {
            curNode = malloc(sizeof(ListNode));
            curNode->next_node = NULL;
            curNode->data = data;
            lastNode->next_node = (struct node *)curNode;
            plist->size++;

            printf("队列刚才生产一条数据，通知阻塞的消费者。目前仓储数量:%d\n", plist->size);
            pthread_cond_signal(&(plist->cond_for_customer));

            // unlock
            pthread_mutex_unlock(&(plist->mutex));
            return;
        }
        lastNode = curNode;
        curNode = (ListNode *)curNode->next_node;
    }
}

int block_queue_pop(List *plist, int *p_data)
{
    // lock
    pthread_mutex_lock(&(plist->mutex));
    while (plist->size <= 0)
    {
        printf("队列已空，消费者阻塞。目前仓储数量:%d\n", plist->size);
        pthread_cond_wait(&(plist->cond_for_customer), &(plist->mutex));
    }

    ListNode *curNode = plist->firstNode;
    if (curNode == NULL)
    {
        printf("队列为空，通知阻塞的生产者。目前仓储数量:%d\n", plist->size);
        pthread_cond_signal(&(plist->cond_for_producer));
        // unlock
        pthread_mutex_unlock(&(plist->mutex));
        return 1; // no data
    }
    else
    {
        *p_data = curNode->data;
        plist->firstNode = (ListNode *)curNode->next_node;
        free(curNode);
        plist->size--;

        printf("队列被消费了一条数据，通知阻塞的生产者。目前仓储数量:%d\n", plist->size);
        pthread_cond_signal(&(plist->cond_for_producer));

        // unlock
        pthread_mutex_unlock(&(plist->mutex));
        return 0;
    }
}
