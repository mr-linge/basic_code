#include <stdio.h>
#include "vaddr_by_symbol.h"


int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
		return -1;
	}
	pid_t pid = atoi(argv[1]);

	size_t func_addr = get_vaddr(pid,"mmap", "libc-",NULL);
	printf("addr : 0x%lx\n",func_addr);
	return 0;
}
