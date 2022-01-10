#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>

size_t getLibcbase(int pid,char * so_name) {
	size_t libcAddr;
	char *buf;
	char *end;
	char mapfile[0x18];
	sprintf(mapfile, "/proc/%d/maps", pid);
	FILE *fd = fopen(mapfile, "r");
	if (!fd) {
		printf("open maps error!");
		exit(1);
	}

	// search the lib.....
	buf = (char *) malloc(0x100);
	do {
		fgets(buf, 0x100, fd);
	} while (!strstr(buf, so_name));

	// get lib address of start
	end = strchr(buf, '-');
	libcAddr = strtol(buf, &end, 16);
	printf("The process %d's libcbase is: 0x%lx\n", pid, libcAddr);

	// get lib path
	char *path = strchr(buf, '/');
	printf("%s path: %s\n", so_name, path);
	
	fclose(fd);

	return libcAddr;
}


int main(int argc, char *argv[]) {
	if (argc < 3) {
		return -1;
	}
	int pid = atoi(argv[1]);
	char * so_name = argv[2];
	size_t addr = getLibcbase(pid, so_name);
	printf("The process %d's (%s) libcbase is: 0x%lx\n",pid,so_name,addr);

	return 0;
}
