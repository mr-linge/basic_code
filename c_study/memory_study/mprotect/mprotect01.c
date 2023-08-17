#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

/*
函数原型如下:
#include <sys/mman.h>
int mprotect(const void *start, size_t len, int prot);

功能描述
修改一段指定内存区域的保护属性.把自start开始的、长度为len的内存区的保护属性修改为prot指定的值

参数介绍
prot可以取以下几个值,并且可以用"|"将几个属性合起来使用:
PROT_READ           表示内存段内的内容可写
PROT_WRITE          表示内存段内的内容可读
PROT_EXEC           表示内存段中的内容可执行
PROT_NONE           表示内存段中的内容根本没法访问

需要指出的是,指定的内存区间必须包含整个内存页(4KB)区间开始的地址start必须是一个内存页的起始地址,并且区间长度len必须是页大小的整数倍

返回值介绍
如果执行成功,则返回0如果执行失败,则返回-1,并且设置errno变量,说明具体因为什么原因造成调用失败错误的原因主要有以下几个:
EACCES              该内存不能设置为相应权限这是可能发生的,比如,如果你 mmap(2) 映射一个文件为只读的,
					接着使用 mprotect() 标志为 PROT_WRITE
EINVAL              start 不是一个有效的指针,指向的不是某个内存页的开头
ENOMEM              内核内部的结构体无法分配
ENOMEM              进程的地址空间在区间 [start, start+len] 范围内是无效,或者有一个或多个内存页没有映射

如果调用进程内存访问行为侵犯了这些设置的保护属性,内核会为该进程产生 SIGSEGV (Segmentation fault 段错误)信号,并且终止该进程
**/

// 获取 addr 所在的 page
#define PAGE_SIZE 0x1000
#define PAGE_START(addr) ((addr) & (~(PAGE_SIZE - 1)))

int main()
{
	/* Allocate a buffer; it will have the default protection of PROT_READ|PROT_WRITE. */
	char *p = (char *)malloc(0x80);
	printf("start address of string : %p\n", p);
	memset(p, 'a', 0x80);
	printf("line : %d, str : %s\n", __LINE__, p);
	char c = p[0]; /* Read; ok */
	printf("line : %d, p[0] : %c\n", __LINE__, c);
	p[0] = 'b'; /* Write; ok */
	printf("line : %d, str : %s\n", __LINE__, p);
	/* Mark the buffer read-only. */
	unsigned long page_start = PAGE_START((unsigned long)p);
	unsigned long page_end = page_start + PAGE_SIZE;
	printf("page start : %lx\n", page_start);
	printf("page end   : %lx\n", page_end);
	int err = mprotect((void *)page_start, PAGE_SIZE, PROT_READ); //  | PROT_WRITE
	if (err != 0)
	{
		perror("mprotect err");
		return -1;
	}
	printf("line : %d, the test string : %s\n", __LINE__, p);
	c = p[0]; /* Read; ok */
	printf("try read : %d, p[0] : %c\n", __LINE__, c);
	p[0] = 'c'; /* Write; program dies on SIGSEGV */
	printf("try write : %d, str : %s\n", __LINE__, p);
	return 0;
}
