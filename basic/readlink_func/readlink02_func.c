#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <errno.h>

int main() {
	char dir[PATH_MAX] = {0}, path[20]={0};
	int pid = 2154;

	sprintf(path, "/proc/%d/exe", pid);
	// dir 保存 进程pid 镜像所在的目录(绝对路径)
	int n = readlink(path, dir, PATH_MAX);
	// printf("path : %s, dir : %s\n",path,dir);
	if(n < 0) {
	  perror("readlink");
	  exit(-1);
	}
	printf("dir: %s\n", dir);

	return 0;
}
