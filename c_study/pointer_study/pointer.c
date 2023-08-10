#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union Data
{
	unsigned char bytes[8];
	unsigned long val;
};

/*
  char *p = (char *)malloc(0x10);
  malloc 	返回的是一个指针,保存在 stack栈 中
  p			分配内存空间的起始地址,在 headp堆 中
  &p		取地址,p指针在 stack栈中保存的地址
  &p[0] 	等价于 p

  *(p + n)	取到 p + n 地址处的数据
*/

int main()
{
	char *p = (char *)malloc(0x10);
	memset(p, 'A', 0x10);
	printf("&p 		vaddr:0x%lx\n", (unsigned long)&p);
	printf("&p 		vaddr:%p\n", &p);
	printf("p 		vaddr:%p\n", p);
	printf("p 		vaddr:0x%lx\n", (unsigned long)p);
	printf("&p[0] 	vaddr:0x%lx\n", (unsigned long)(&p[0]));
	union Data dt;
	memcpy(dt.bytes, (void *)&p, 8);
	printf("dt.val:0x%lx\n", dt.val);

	for (int i = 0; i < 0x10; i++)
	{
		p[i] = (char)(65 + i);
		printf("%p --> %c\t", &p[i], p[i]);
		if ((i + 1) % 0x4 == 0)
		{
			puts("");
		}
	}
	puts("");
}
