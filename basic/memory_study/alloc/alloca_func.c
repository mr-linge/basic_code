#include <stdio.h>
#include <string.h>
#include <alloca.h>

/****************
#include <alloca.h>
void *alloca(size_t size);
alloca() 是一个基于栈进行内存分配的函数。

该函数成功时，会返回一个指向 size 字节大小的内存指针。这块内存是在栈中的。所以，从函数返回时，它会被自动释放。失败时（size 过大），可能会存在栈溢出问题（可能会指向堆区或者其它区域）。分配速度快，但是可移植性差。
 *****************/

char *str;

void func() {
	str = alloca(16);
	memset(str,'a',16);
	str[15] = '\0';

	printf("%s\n",__FUNCTION__);
	printf("str : %s, addr : %p\n",str, str);
}

// 破坏原来的stack
void func1() {
	char * block = alloca(1024);
	memset(block,'d',1024);
	printf("%s\n",__FUNCTION__ );
	printf("str : %s, addr : %p\n",str, str);
}

int main() {
	func();
	func1();
	printf("%s\n",__FUNCTION__);
	printf("str : %s, addr : %p\n",str, str);

	return 0;
}

