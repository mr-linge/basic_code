#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>
#include <dlfcn.h>

#define path "./loaded_file"

/***************
  可以通过 dlsym 获取每一个加载进当前进程模块中的任何一个符号，返回值是符号所在的地址。但是对当前进程image里所定义的本地符号是无效的(至少我暂时还不能根据dlsym 获取local symbol,个人认为dlsym只能获取动态库xxx.so定义的symbol).
  可以把这个返回的地址 转换成 对应的函数指针。如果个地址所在的空间有执行权限,通过函数指针调用这个函数，效果和直接调用这个函数一样
  dlsym(0,symbol) 第一个参数传 0 表示 在当前进程所在的模块寻找符号
 * *****************/

void origin_mmap() {
	int fd;
	void * start;
	struct stat sb;
	fd = open(path, O_RDONLY); /* 打开/etc/passwd */
	fstat(fd, &sb); /* 取得文件大小 */
	start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(start == MAP_FAILED) /* 判断是否映射成功 */
	{
		perror("mmap init fail");
		exit(-1);
	}
	printf("start = %p, value = %s\n",start,(char *)start);
	munmap(start, sb.st_size); /* 解除映射 */
	close(fd);
}

typedef void *(*MY_MMAP)(void *start, size_t length, int prot, int flags, int fd, off_t offsize);

void get_mmap_addr() {
	MY_MMAP my_mmap = NULL;
	printf("&mmap	:%p\n",&mmap);
	my_mmap =(MY_MMAP)&mmap;
	printf("my_mmap:%p\n",(void *)my_mmap);

	int fd;
	void * start;
	struct stat sb;
	fd = open(path, O_RDONLY); /* 打开/etc/passwd */
	fstat(fd, &sb); /* 取得文件大小 */
	start = my_mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(start == MAP_FAILED) /* 判断是否映射成功 */
	{
		perror("mmap init fail");
		exit(-1);
	}
	printf("start = %p, value = %s\n",start,(char *)start);
	munmap(start, sb.st_size); /* 解除映射 */
	close(fd);
}

void my_by_dlsym() {
	MY_MMAP my_mmap2 = NULL;
	my_mmap2 = (MY_MMAP)dlsym(0, "mmap");
	printf("my_mmap2:%p\n",(void *)my_mmap2);

	int fd;
	void * start;
	struct stat sb;
	fd = open(path, O_RDONLY); /* 打开/etc/passwd */
	fstat(fd, &sb); /* 取得文件大小 */
	start = my_mmap2(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(start == MAP_FAILED) /* 判断是否映射成功 */
	{
		perror("mmap init fail");
		exit(-1);
	}
	printf("start = %p, value = %s\n",start,(char *)start);
	munmap(start, sb.st_size); /* 解除映射 */
	close(fd);
}

/**
 * gcc dlsym01_func.c -o main -ldl
 * **/
int main() {
	origin_mmap();
	printf("*****************%d*******************\n",__LINE__);
	get_mmap_addr();
	printf("*****************%d*******************\n",__LINE__);
	my_by_dlsym();

	return 0;
}
