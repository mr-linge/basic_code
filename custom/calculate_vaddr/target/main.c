#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>

typedef void *(*MY_MMAP)(void *start, size_t length, int prot, int flags, int fd, off_t offsize);

int main()
{
	puts("call hello()");
	hello();
	void (*cac_func)(void) = (void (*)()) & hello;
	cac_func();

	void *(*my_mmap)(void *, size_t, int, int, int, off_t) = (void *)&mmap;
	MY_MMAP my_mmap2 = (MY_MMAP)dlsym(0, "mmap");
	while (1)
	{
		printf("cac_func        addr: %p\n", (void *)cac_func);
		printf("mmap            addr: %p\n", (void *)mmap);
		printf("my_mmap         addr: %p\n", (void *)my_mmap);
		printf("my_mmap2        addr: %p\n", (void *)my_mmap2);
		printf("pid : %d. wait a minutes ...\n", getpid());
		sleep(10);
	}

	return 0;
}
