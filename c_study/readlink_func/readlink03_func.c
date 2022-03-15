#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>

int main(int argc, char * argv[]) {
	if(argc < 2) {
		printf("Please input your pid ...\n");
		return -1;
	}

	int pid = atoi(argv[1]);
	if(pid == 0) {
		printf("%s is Illegal parameter\n", argv[1]);
		exit(-1);
	}

	char dir[PATH_MAX] = {0}, path[20]={0};
	sprintf(path, "/proc/%d/exe", pid);
	int n = readlink(path, dir, PATH_MAX);
	if(n < 0) {
		perror("readlink");
		exit(-1);
	}
	printf("dir: %s\n", dir); 

	return 0;
}
