#include <stdio.h>
#include <elf.h>

int main(int argc, char * argv[]) {
	long len = sizeof(Elf64_Addr);
	printf("len : %ld\n", len);
	printf("len : %ld\n", sizeof(Elf64_Half));
	printf("len : %ld\n", sizeof(Elf64_Sword));
	printf("len : %ld\n", sizeof(Elf64_Xword));
	return 0;
}
