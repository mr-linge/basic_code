#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

/*
   #include <unistd.h>
   #include <sys/mmap.h>
   int mprotect(const void *start, size_t len, int prot);
**/

// 获取 addr 页的上边界和下边界地址(页的上边界)
#define PAGE_SHIFT 12
#define PAGE_SIZE (1UL << PAGE_SHIFT)
#define PAGE_MASK (~(PAGE_SIZE - 1))

#define PAGE_START(addr) ((addr)&PAGE_MASK)
#define PAGE_END(addr) (PAGE_START(addr) + PAGE_SIZE)

int main()
{
	/* Allocate a buffer; it will have the default
	   protection of PROT_READ|PROT_WRITE. */
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
	unsigned long page_end = PAGE_END((unsigned long)p);
	printf("page start : %lx\n", page_start);
	printf("page end   : %lx\n", page_end);
	int err = mprotect((void *)page_start, PAGE_SIZE, PROT_READ); //  | PROT_WRITE
	if (err < 0)
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
