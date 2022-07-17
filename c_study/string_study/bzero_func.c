#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
   头文件：#include <string.h>

   bzero() 会将内存块（字符串）的前n个字节清零，其原型为：
   void bzero(void *s, int n);

   【参数】s为内存（字符串）指针，n 为需要清零的字节数。

   bzero()会将参数s 所指的内存区域前n 个字节，全部设为零值。

   实际上，bzero(void *s, int n) 等价于 memset((void*)s, 0,size_tn)，用来将内存块的前 n 个字节清零，但是 s 参数为指针，又很奇怪的位于 string.h 文件中，也可以用来清零字符串。

   注意：bzero() 不是标准函数，没有在ANSI中定义，笔者在VC6.0和MinGW5下编译没通过；据称Linux下的GCC支持，不过笔者没有亲测。鉴于此，还是使用 memset() 替代吧。
 */

int main() {
	int len = 0x100;
	char *s = (char *)malloc(len);

	bzero(s, len);  // struct initialization to zero
	
	char *str = "Hello, will be success!";
	memcpy(s,str,strlen(str));
	printf("s:%s\n",s);	

	return 0;
}
