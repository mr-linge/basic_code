#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

// 把数据写到指定路径的目录里,如果目录不存在则创里的内容
void write_message(char *path, char *data, unsigned long len)
{
	int fd = open(path, O_WRONLY | O_CREAT);
	if (fd < 0)
	{
		perror("create directory fail!");
		exit(-1);
	}
	int size = write(fd, data, len);
	if (size < 0)
	{
		perror("open file fail!");
		exit(1);
	}
	close(fd);
}

void common_func(char *path)
{
	char *new_path = (char *)calloc(1024, 1);
	strcat(new_path, path);
	strcat(new_path, ".temp");
	printf("new_path : \n%s\n", new_path);
	char *blocks = "AAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
	write_message(new_path, blocks, 20);
	// int ret = remove(path);//删除文件
	// if(ret < 0) {
	//         perror("delete directory fail!");
	//         exit(1);
	// }
	free(new_path);
	//        free(path);
}

void main()
{
	char path2[] = "/home/dio/Repositories/C_study/my_func/crypto/test/";
	char *path = (char *)calloc(0x100, 1);
	strncpy(path, path2, sizeof(path2));

	for (int i = 0; i < 20; i++)
	{
		char c = (char)('a' + i);
		char s[3] = {c, c, '\0'};
		printf("s : %s\n", s);
		strcat(path, s);
		common_func(path);
	}
}
