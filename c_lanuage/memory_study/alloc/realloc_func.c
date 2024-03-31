#include <stdio.h>
#include <stdlib.h>

/*
Function: void * realloc(void * ptr, unsigned new_size);
Param:
	ptr 			需要重新分配空间的指针
	new_size			新的空间大小

Description: 为 ptr 指针重新分配大小为 new_size 的内存空间,并不保证调整后的内存空间和原来的内存空间起始地址相同.返回的指针可能指向一个新的地址
Return:
   1. 如果ptr为NULL,则函数相当于malloc(new_size),试着分配一块大小为new_size的内存, 成功将地址返回,失败返回 NULL。
	  如果 ptr不为 NULL,则进入 2
   2. 如果 ptr 不在 heap 中, 会抛出realloc invalid pointer异常

	  ptr 在 heap 中
	  如果 new_size = 0, 相当于free(ptr), 将ptr指向的内存空间释放掉,返回NULL
	  如果 new_size < origin_size, 则ptr 指向的数据可能会丢失,只有new_size大小的数据会保存;
	  如果 new_size = origin_size, 等于什么都没有做;

	  如果 new_size > origin_size, 则查看ptr指向的位置还有没有足够的连续内存空间
	  如果有的话,分配更多的空间,返回的地址和ptr相同
	  如果没有的话,在更大的空间内查找,找到size大小的空间,将旧的内容拷贝到新的内存中,把旧的内存释放掉,返回新地址

	  如果内存中没有 new_size  大小的空间则无法再次分配,返回NULL
**/

int main()
{
	char *ptr;
	// 最初的内存分配
	ptr = (char *)malloc(0x10);
	if (ptr)
	{
		printf("Memory Allocated at: %p\n", ptr);
	}
	else
	{
		printf("Not Enough Memory!\n");
	}

	// 重新分配内存
	ptr = (char *)realloc(ptr, 0x100);
	if (ptr)
	{
		printf("Memory Reallocated at: %p\n", ptr);
	}
	else
	{
		printf("Not Enough Memory!\n");
	}

	free(ptr);

	return 0;
}
