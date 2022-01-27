#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

/****************
   #include <unistd.h>
   #include <sys/mmap.h>
   
   int mprotect(const void *start, size_t len, int prot);
 * *****************/

// 将地址addr修整为页边界地址(页的上边界)
#define PAGE_SHIFT          12
#define PAGE_SIZE           (1UL << PAGE_SHIFT)
#define PAGE_MASK           (~(PAGE_SIZE-1))

#define PAGE_START(addr) ((addr) & PAGE_MASK)
#define PAGE_END(addr)   (PAGE_START(addr) + PAGE_SIZE)

int main() {
	/* Allocate a buffer; it will have the default
	   protection of PROT_READ|PROT_WRITE. */
	char *p = (char *) malloc(128);
	printf("p : %p\n", p);
	memset(p, 'a', 128);
	printf("line : %d, str : %s\n", __LINE__, p);
	char c = p[0];         /* Read; ok */
	printf("line : %d, str : %s\n", __LINE__, p);
	p[1] = 'b';        /* Write; ok */
	printf("line : %d, str : %s\n", __LINE__, p);
	/* Mark the buffer read-only. */
	char *p2 = (char *) PAGE_START((unsigned long) p);
	printf("p2 : %p\n", p2);
	int err = mprotect(p2, PAGE_SIZE, PROT_READ); //  | PROT_WRITE
	if (err < 0) {
		perror("mprotect err:");
		return -1;
	}
	printf("line : %d, str : %s\n", __LINE__, p);
	c = p[2];         /* Read; ok */
	printf("line : %d, str : %s\n", __LINE__, p);
	p[3] = 'c';        /* Write; program dies on SIGSEGV */
	printf("line : %d, str : %s\n", __LINE__, p);

	return 0;
}
