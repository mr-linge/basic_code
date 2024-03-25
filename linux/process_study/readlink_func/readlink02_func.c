#include <stdio.h>
#include <unistd.h>

int main()
{
	char dir[0x100] = {0}, path[0x100] = {0};
	int pid = getpid();

	sprintf(path, "/proc/%d/exe", pid);
	// dir 保存 进程pid 镜像所在的目录(绝对路径)
	int n = readlink(path, dir, sizeof(dir));
	// printf("path : %s, dir : %s\n",path,dir);
	if (n < 0)
	{
		perror("readlink");
		return -1;
	}
	printf("dir: %s\n", dir);

	return 0;
}
