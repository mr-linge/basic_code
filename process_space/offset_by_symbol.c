#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <elf.h>

#include <sys/stat.h>
#include <sys/mman.h>


typedef struct handle {
	Elf64_Ehdr *ehdr;
	Elf64_Phdr *phdr;
	Elf64_Shdr *shdr;
	uint8_t *mem;
} handle_t;


/**
 * 根据符号名在 .symtab 查找符号地址
 */
Elf64_Addr lookup_symbol_symtab(handle_t *h,char *symbol,int bind,int type) {
	unsigned long i, j, NumOfSym;
	char *strtab;
	Elf64_Sym *symtab;

	for (i = 0; i < h->ehdr->e_shnum; i++) {
		/* 找 .symtab 节，该节保存了符号信息，每一个符号项为Elf64_Sym */
		if (h->shdr[i].sh_type == SHT_SYMTAB) {
			/* 类型为SHT_SYMTAB的节，其sh_link为字符串表所在节表中的下标 */
			/* 因此h->shdr[i].sh_link为字符串表下标 */
			strtab = (char *) (h->mem + h->shdr[h->shdr[i].sh_link].sh_offset);

			/* 获取符号表的首地址 */
			symtab = (Elf64_Sym *) &h->mem[h->shdr[i].sh_offset];

			NumOfSym = h->shdr[i].sh_size / sizeof(Elf64_Sym);

			for (j = 0; j < NumOfSym; j++) {
				/* st_name为符号名在字符串表中的下标 */
				char *name = (char *) &strtab[symtab->st_name];
				//printf("symtab->st_info : %d\n", symtab->st_info);
				//printf("symbol name : %s\n", name);
				int name_right = !strncmp(name, symbol, strlen(symbol));
				int st_info = ELF64_ST_INFO(bind,type);
				//printf("st_info : %d\n", st_info);
				int st_info_right = (st_info == symtab->st_info);
				if (name_right && st_info_right) {
					/* st_value 为符号的地址 */
					return symtab->st_value;
				}
				symtab++;
			}
		}
	}

	return 0;
}

/**
 * 根据符号名在 .dynsym 查找符号地址
 */
Elf64_Addr lookup_symbol_dynsym(handle_t *h,char *symbol,int bind,int type) {
	unsigned long i, j, NumOfSym;
	char *dynstr;
	Elf64_Sym *symtab;

	for (i = 0; i < h->ehdr->e_shnum; i++) {
		/* 找 .symtab 节，该节保存了符号信息，每一个符号项为Elf64_Sym */
		if (h->shdr[i].sh_type == SHT_DYNSYM) {
			/* 类型为SHT_SYMTAB的节，其sh_link为字符串表所在节表中的下标 */
			/* 因此h->shdr[i].sh_link为字符串表下标 */
			dynstr = (char *) (h->mem + h->shdr[h->shdr[i].sh_link].sh_offset);

			/* 获取符号表的首地址 */
			symtab = (Elf64_Sym *) &h->mem[h->shdr[i].sh_offset];

			NumOfSym = h->shdr[i].sh_size / sizeof(Elf64_Sym);

			for (j = 0; j < NumOfSym; j++) {
				/* st_name为符号名在字符串表中的下标 */
				char *name = (char *) &dynstr[symtab->st_name];
				//printf("symtab->st_info : %d\n", symtab->st_info);
				//printf("symbol name : %s\n", name);
				int name_right = !strncmp(name, symbol, strlen(symbol));
				int st_info = ELF64_ST_INFO(bind,type);
				//printf("st_info : %d\n", st_info);
				int st_info_right = (st_info == symtab->st_info);
				if (name_right && st_info_right) {
					/* st_value 为符号的地址 */
					return symtab->st_value;
				}
				symtab++;
			}
		}
	}

	return 0;
}

handle_t init_elf_file(char *path) {
	handle_t h;
	struct stat st;

	/* 打开指定文件 */
	int fd;
	if ((fd = open(path, O_RDONLY)) < 0) {
		perror("open");
		exit(-1);
	}

	/* 获取指定文件的属性 */
	if (fstat(fd, &st) < 0) {
		perror("fstat");
		exit(-1);
	}

	/* 将目标文件映射到本进程的虚拟内存中 */
	h.mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (h.mem == MAP_FAILED) {
		perror("mmap");
		exit(-1);
	}

	h.ehdr = (Elf64_Ehdr *) h.mem;
	h.phdr = (Elf64_Phdr *) (h.mem + h.ehdr->e_phoff);
	h.shdr = (Elf64_Shdr *) (h.mem + h.ehdr->e_shoff);

	return h;
}

void test_symtab(handle_t h,char *symbol) {
	Elf64_Addr addr = lookup_symbol_symtab(&h,symbol,STB_GLOBAL,STT_FUNC);
	printf("symtab func:%s, addr:0x%lx\n", symbol, addr);
}

void test_dynsym(handle_t h,char *symbol) {
	Elf64_Addr addr = lookup_symbol_dynsym(&h,symbol,STB_GLOBAL,STT_FUNC);
	printf("dynsym func:%s, addr:0x%lx\n", symbol, addr);
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("params is less than 3 ..");
		return -1;
	}

	char *path = argv[1];
	handle_t h = init_elf_file(path);

	char *symbol = argv[2];

	test_symtab(h,symbol);
	printf("************************************************\n");
	test_dynsym(h,symbol);

	return 0;
}
