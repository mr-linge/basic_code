#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PATH_MAX 0x1000

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Please input your pid ...\n");
		return -1;
	}

	int pid = atoi(argv[1]);
	if (pid == 0)
	{
		printf("%s is Illegal parameter\n", argv[1]);
		return -2;
	}

	char dir[PATH_MAX] = {0}, path[0x100] = {0};
	sprintf(path, "/proc/%d/exe", pid);
	int n = readlink(path, dir, PATH_MAX);
	if (n < 0)
	{
		perror("readlink");
		return -3;
	}
	printf("dir: %s\n", dir);

	return 0;
}
