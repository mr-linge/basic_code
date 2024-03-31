#include <stdio.h>
#include <string.h>
#include <alloca.h>

/*
Header #include <alloca.h>
Function: void * alloca(size_t size);
Description: 在栈分配内存

Return:
成功,会返回一个指向 size 字节大小的内存指针。这块内存是在栈中的,不需要手动释放.
失败(size 过大),可能会存在栈溢出问题(可能会指向堆区或者其它区域)。分配速度快,但是可移植性差。
**/

char *ptr;

void func()
{
	ptr = alloca(16);
	memset(ptr, 'a', 16);
	ptr[15] = '\0';

	printf("%d ptr:%s, addr:%p\n", __LINE__, ptr, ptr);
}

// 破坏原来的stack
void func1()
{
	char *block = alloca(1024);
	memset(block, 'd', 1024);
	printf("%d ptr:%s, addr:%p\n", __LINE__, ptr, ptr);
}

int main()
{
	func();
	func1();
	printf("%d ptr:%s, addr:%p\n", __LINE__, ptr, ptr);

	return 0;
}
