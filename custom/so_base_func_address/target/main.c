#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

//typedef void(*CAC_FUNC)(void);

int main() {
	//CAC_FUNC cac_func = NULL;
	puts("call hello()");
	hello();
	void(*cac_func)(void) = hello;
	//printf("cac_func addr: %p\n", cac_func);

	int fd;
	void * start;
	struct stat sb;
	fd = open("/etc/profile", O_RDONLY); /* 打开/etc/passwd */
	fstat(fd, &sb); /* 取得文件大小 */
	start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(start == MAP_FAILED) /* 判断是否映射成功 */
	{
		perror("mmap init fail");
		exit(-1);
	}

	void(**my_mmap)(void *, size_t, int, int, int, off_t) = mmap;
	size_t my_mmap2 = (size_t)dlsym(0, "mmap");
        while(1) {
                printf("cac_func        addr: %p\n", (void *)cac_func);
                printf("mmap            addr: %p\n", (void *)mmap);
                printf("my_mmap         addr: %p\n", (void *)my_mmap);
                printf("my_mmap2        addr: %p\n", (void *)my_mmap2);
                printf("pid : %d. wait a minutes ...\n", getpid());
                sleep(10);
        }

	return 0;
}

