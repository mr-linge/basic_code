#include <stdio.h>
#include <stdlib.h>

/*
   函数原型：
   void* realloc(void* ptr, unsigned newsize);
   函数功能：
   使用realloc函数为ptr重新分配大小为size的一块内存空间。下面是这个函数的工作流程：
   1. 对ptr进行判断，如果ptr为NULL，则函数相当于malloc(new_size)，试着分配一块大小为new_size的内存，如果成功将地址返回，否则返回NULL。如果ptr不为NULL，则进入(2)。
   2. 查看ptr是不是在堆中，如果不是的话会抛出realloc invalid pointer异常。如果ptr在堆中，则查看new_size大小，如果new_size大小为0，则相当于free(ptr)，将ptr指向的内存空间释放掉，返回NULL。如果new_size小于原大小，则ptr中的数据可能会丢失，只有new_size大小的数据会保存；如果size等于原大小，等于什么都没有做；如果size大于原大小，则查看ptr指向的位置还有没有足够的连续内存空间，如果有的话，分配更多的空间，返回的地址和ptr相同，如果没有的话，在更大的空间内查找，如果找到size大小的空间，将旧的内容拷贝到新的内存中，把旧的内存释放掉，则返回新地址，否则返回NULL。
   3. realloc对给定的指针所指的空间进行扩大或者缩小，无论是扩张或是缩小，原有内存的中内容将保持不变。当然，对于缩小，则被缩小的那一部分的内容会丢失。
   4. realloc 并不保证调整后的内存空间和原来的内存空间保持同一内存地址。相反，realloc 返回的指针很可能指向一个新的地址。所以在代码中，我们必须将realloc返回的值，重新赋值
 * */

int main () {
	char *p;
	p=(char *)malloc(100);
	if(p)
		printf("Memory Allocated at: %p\n",p);
	else
		printf("Not Enough Memory!/n");
	char *new_p=(char *)realloc(p,256);
	if(new_p) {
		printf("Memory Reallocated at: %p\n",new_p);
	} else {
		printf("Not Enough Memory!/n");
		free(p);
		p = NULL;
		return -1;
	}
	p = new_p;

	free(p);

	return 0;
}
