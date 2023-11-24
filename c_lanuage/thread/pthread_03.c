#include <stdio.h>
#include <stdlib.h>
#include "block_queue.h"

#define BLOCK_POOL_SIZE 10

void producer(void *arg);
void customer(void *arg);

int main() {
    int pthread_ret;
    pthread_t producer_thread_0,producer_thread_1,customer_thread;
    List *plist = block_queue_create(BLOCK_POOL_SIZE);

    pthread_ret = pthread_create(&producer_thread_0,NULL,(void *)producer,plist);
    if(pthread_ret!=0){
        puts("生产者线程创建失败");
        return EXIT_FAILURE;
    }

    pthread_ret = pthread_create(&producer_thread_1,NULL,(void *)producer,plist);
    if(pthread_ret!=0){
        puts("生产者线程创建失败");
        return EXIT_FAILURE;
    }

    pthread_ret = pthread_create(&customer_thread,NULL,(void *)customer,plist);
    if(pthread_ret!=0){
        puts("消费者线程创建失败");
        return EXIT_FAILURE;
    }

    // 主线程等待.
    pthread_join(producer_thread_0,NULL);
    pthread_join(producer_thread_1,NULL);
    pthread_join(customer_thread,NULL);

    block_queue_destroy(plist);
    return EXIT_SUCCESS;
}

void producer(void *arg){
    List *plist = (List *)arg;
    for (int i = 0; i < 40; i++) {
        block_queue_push(plist,i);
        printf("生产数据:%d\n",i);
    }

    printf("---- 生产结束 ----\n");
    pthread_exit(NULL);
}

void customer(void *arg){
    int data;
    List *plist = (List *)arg;
    for (int i = 0; i < 80; i++) {
        block_queue_pop(plist,&data);
        printf("消费数据:%d\n",data);
    }

    printf("---- 消费结束 ----\n");
    pthread_exit(NULL);
}
