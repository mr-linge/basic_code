#include <stdio.h>
#include <pthread.h>

void add_1(void *arg);
pthread_mutex_t mutex;

int main()
{
	int i = 10;

	// 声明线程
	pthread_t thread1, thread2;

	// 初始化锁
	pthread_mutex_init(&mutex, NULL);

	// 创建线程
	int rc1 = pthread_create(&thread1, NULL, (void *)add_1, &i);
	if (rc1 != 0)
	{
		printf("thread-1 create failed: %d\n", rc1);
	}

	// 创建线程
	int rc2 = pthread_create(&thread2, NULL, (void *)add_1, &i);
	if (rc2 != 0)
	{
		printf("thread-2 create failed: %d\n", rc1);
	}

	// 等待线程
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("i = %d\n", i); // 打印输出 i = 2000010, 如果不加锁打印的值可能是别的

	return 0;
}

// 线程运行函数
void add_1(void *arg)
{
	int *i = (int *)arg;
	for (int n = 0; n < 1000000; n++)
	{
		pthread_mutex_lock(&mutex);
		(*i)++;
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}
