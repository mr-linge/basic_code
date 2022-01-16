#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./symbol_offset.h"
#include "./so_base.h"

int main(int argc, char *argv[]) {
	if (argc < 4) {
		return -1;
	}
	int pid = atoi(argv[1]);
	char *so_name = argv[2];
	char *symbol_name = argv[3];

	char so_path[256];
	size_t base_addr = getLibcbase(pid, so_name, so_path);
	printf("The process %d's libcbase is: 0x%lx\n", pid, base_addr);

	size_t offset = symbol_address(so_path, symbol_name);
	printf("offset: 	0x%lx\n", offset);

	size_t vaddr = base_addr + offset;
	printf("vaddr: 	0x%lx\n", vaddr);

	return 0;
}
