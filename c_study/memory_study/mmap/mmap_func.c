#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>

int main()
{
	int fd, ret;
	void *start;
	struct stat sb;
	fd = open("./loaded_file", O_RDONLY); /* 打开/etc/passwd */
	fstat(fd, &sb);						  /* 取得文件大小 */
	start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (start == MAP_FAILED) /* 判断是否映射成功 */
	{
		perror("mmap init fail");
		exit(-1);
	}
	printf("start = %p, value = %s\n", start, (char *)start);

	ret = msync((void *)start, sb.st_size, MS_SYNC);
	if (ret == -1)
	{
		perror("msync error");
	}

	ret = munmap(start, sb.st_size); /* 解除映射 */
	if (ret == -1)
	{
		perror("参数 start或length 不合法");
	}
	close(fd);
	//	printf("start = %p, value = %s\n",start,(char *)start);

	return 0;
}
