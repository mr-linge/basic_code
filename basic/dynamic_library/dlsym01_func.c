#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>

#define path "./loaded_file"

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
	MY_MMAP my_mmap =(MY_MMAP)&mmap;
	
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

void by_dlsym() {
	MY_MMAP my_mmap2 = (MY_MMAP)dlsym(0, "mmap");

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

int main() {
	//origin_mmap();
//	get_mmap_addr();
	by_dlsym();	
//	char *funcName = "mmap";
//	size_t funcAddr = (size_t)dlsym(0, funcName);

	return 0;
}
