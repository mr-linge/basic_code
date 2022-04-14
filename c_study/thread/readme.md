1 线程基础
1.1 创建线程
pthread_create

#include <pthread.h>
int pthread_create(pthread_t *restrict tidp, const pthread_attr_t *restrict attr, void *(*start_rtn)(void),  void *restrict arg);
【参数】

tidp：传入函数，该函数会把创建的线程指针赋值给该参数。
attr：线程属性。
start_rtn：启动线程的函数指针。
arg：运行函数的参数。
【返回值】成功，返回0；出错，返回-1。

 

1.2 线程挂起
pthread_join

#include <pthread.h>
int pthread_join( pthread_t thread, void **value_ptr);
让当前线程在等待指定线程执行完后再继续执行。

参数：

thread：等待退出线程的线程号。
value_ptr：退出线程的返回值。
 

1.3 线程退出
pthread_exit

#include <pthread.h>
void pthread_exit(void *rval_ptr);
退出当前线程。该函数用于线程函数主动退出，而不是让其他线程退出。

 

1.4 获取当前线程id
pthread_self

#include <pthread.h>
pthread_t pthread_self(void);
获取当前线程。线程ID由是pthread_t类型表示。

 

1.5 判断线程是否相等
pthread_equal

#include <pthread.h>
int pthread_equal(pthread_t tid1, pthread_t tid2);
检查两个pthread是否相等。

在不同的系统下，pthread_t的类型是不同的，比如在ubuntn下，是unsigned long类型，而在solaris系统中，是unsigned int类型。而在FreeBSD上才用的是结构题指针。 所以不能直接使用==判读，而应该使用pthread_equal来判断。

1.6 线程执行函数的返回值
pthread_exit(void *ret)
pthread_join(pthread_t id, void **ret)
函数退出时，由pthread_exit函数退出，并用ret传出真正的返回值。外部函数若想接受到该参数，使用pthread_join的第二个参数来接收。

1.7 回收线程资源
pthread_detach

#include <pthread.h>
int pthread_detach(pthread_t);
线程分离。

Linux的线程有两种状态：

joinable态
unjoinable态
默认线程是joinable的，则当线程函数自己返回退出时或pthread_exit时都不会释放线程所占用堆栈和线程描述符（总计8K多）。只有对该线程调用了pthread_join之后这些资源才会被释放。

线程的unjoinable状态可以在pthread_create时指定，也可以在线程创建后在线程中pthread_detach自己, 确保资源的释放。如：pthread_detach(pthread_self())。

一般的回收线程有两种方法：

方法一：使用pthread_detach让线程自动回收资源。

pthread_t tid;
int status = pthread_create(&tid, NULL, ThreadFunc, NULL);
if(status != 0){
    perror("pthread_create error");
}
pthread_detach(tid);
 

方法二：使用thread_join让下一个线程回收当前线程。


2 互斥量（锁）
2.1 创建互斥量
静态初始化互斥量

PTHREAD_MUTEX_INITIALIZER

pthread_mutex_t theMutex;
pthread_mutex_t result = PTHREAD_MUTEX_INITIALIZER;
 

动态初始化互斥量

pthread_mutex_init

#include <pthread.h>
int  pthread_mutex_init(pthread_mutex_t  *restrict_mutex,const pthread_mutextattr_t *restrict attr)

2.2 销毁锁
pthread_mutex_destroy

#include <pthread.h>
int pthread_mutex_destroy(pthread_mutex_t *mutex);
使用完后释放。

2.3 加锁
pthread_mutex_lock

#include <pthread.h>
int pthread_mutex_lock(pthread_mutex_t *mutex); 

2.4 尝试加锁
pthread_mutex_trylock

#include <pthread.h>
int pthread_mutex_trylock( pthread_mutex_t *mutex );
加锁，但是与pthread_mutex_lock不一样的是当锁已经在使用的时候，返回为EBUSY，而不是挂起等待。

2.5 在一定时间内加锁
pthread_mutex_timedlock

#include <pthread.h>
int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex, const struct timespec *restrict abs_timeout); 
尝试lock，abs_timeout时间内仍被锁定，返回ETIMEOUT错误。timespec是超时时间，它用秒和纳秒来描述时间。

2.6 解锁
pthread_mutex_unlock

#include <pthread.h>
pthread_mutex_unlock(pthread_mutex_t *mutex);
释放锁。


4 条件锁
4.1 静态创建
PTHREAD_COND_INITIALIZER

#include <pthread.h>
pthread_cond_t my_condition = PTHREAD_COND_INITIALIZER;
 

4.2 动态创建
pthread_cond_init

#include <pthread.h>
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
 

4.3 销毁
pthread_cond_destroy

#include <pthread.h>
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
 

4.4 等待
pthread_cond_wait

#include <pthread.h>
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);
 

4.5 触发
pthread_cond_signal

#include <pthread.h>
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
