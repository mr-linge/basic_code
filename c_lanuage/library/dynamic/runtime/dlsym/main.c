#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <dlfcn.h>

#define filepath "./loaded_file"

void origin_mmap()
{
	int fd;
	void *start;
	struct stat sb;
	fd = open(filepath, O_RDONLY); /* 打开文件 */
	fstat(fd, &sb);				   /* 取得文件大小 */
	start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (start == MAP_FAILED) /* 判断是否映射成功 */
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(-1);
	}
	printf("start = %p, value = %s\n", start, (char *)start);
	munmap(start, sb.st_size); /* 解除映射 */
	close(fd);
}

typedef void *(*MY_MMAP)(void *start, size_t length, int prot, int flags, int fd, off_t offsize);

void get_mmap_addr()
{
	MY_MMAP my_mmap = NULL;
	printf("&mmap	:%p\n", &mmap);
	my_mmap = (MY_MMAP)&mmap;
	printf("my_mmap:%p\n", (void *)my_mmap);

	int fd;
	void *start;
	struct stat sb;
	fd = open(filepath, O_RDONLY); /* 打开文件 */
	fstat(fd, &sb);				   /* 取得文件大小 */
	start = my_mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (start == MAP_FAILED) /* 判断是否映射成功 */
	{
		perror("mmap init fail");
		exit(-1);
	}
	printf("start = %p, value = %s\n", start, (char *)start);
	munmap(start, sb.st_size); /* 解除映射 */
	close(fd);
}

void my_by_dlsym()
{
	char *error;
	MY_MMAP my_mmap2 = NULL;
	my_mmap2 = (MY_MMAP)dlsym(0, "mmap");
	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, error);
		exit(1);
	}
	printf("my_mmap2:%p\n", (void *)my_mmap2);

	int fd;
	void *start;
	struct stat sb;
	fd = open(filepath, O_RDONLY); /* 打开文件 */
	fstat(fd, &sb);				   /* 取得文件大小 */
	start = my_mmap2(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (start == MAP_FAILED) /* 判断是否映射成功 */
	{
		perror("mmap init fail");
		exit(-1);
	}
	printf("start = %p, value = %s\n", start, (char *)start);
	munmap(start, sb.st_size); /* 解除映射 */
	close(fd);
}

int main()
{
	origin_mmap();
	printf("*****************%d*******************\n", __LINE__);
	get_mmap_addr();
	printf("*****************%d*******************\n", __LINE__);
	my_by_dlsym();
	return 0;
}
