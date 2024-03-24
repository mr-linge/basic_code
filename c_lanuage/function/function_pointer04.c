#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *test_mmap(void *start, size_t length, int prot, int flags, int fd, off_t offsize)
{
	char *str = (char *)malloc(16);
	memset(str, 'a', 15);
	return (void *)str;
}

typedef void *(*MY_MMAP)(void *start, size_t length, int prot, int flags, int fd, off_t offsize);

int main()
{
	char *str1 = test_mmap(NULL, 0, 0, 0, 0, 0);
	printf("test_mmap ret str1 : %s\n", str1);

	printf("*****************%d*******************\n", __LINE__);

	void *(*my_mmap)(void *start, size_t length, int prot, int flags, int fd, off_t offsize) = (void *)test_mmap;
	printf("my_mmap addr: %p\n", (void *)my_mmap);
	char *str2 = (char *)my_mmap(NULL, 0, 0, 0, 0, 0);
	printf("test_mmap ret str2 : %s\n", str2);

	printf("*****************%d*******************\n", __LINE__);

	MY_MMAP my_mmap2 = NULL;
	my_mmap2 = (MY_MMAP)(&test_mmap);
	printf("my_mmap2 addr: %p\n", my_mmap2);
	char *str3 = (char *)my_mmap2(NULL, 0, 0, 0, 0, 0);
	printf("test_mmap ret str3 : %s\n", str3);

	return 0;
}
