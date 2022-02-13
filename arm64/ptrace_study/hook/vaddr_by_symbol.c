#include "vaddr_by_symbol.h"

// 计算模块在进程中的虚拟地址(即so加载到进程后的首地址)
unsigned long get_module_base(int pid, char *module)
{
	unsigned long moduleAddr;
	char *buf;
	char mapfile[0x18];
	sprintf(mapfile, "/proc/%d/maps", pid);
	FILE *fd = fopen(mapfile, "r");
	if (!fd)
	{
		printf("open maps error!\n");
		exit(1);
	}
	// search the libc-.....
	buf = (char *)malloc(0x100);
	unsigned long loop_num = 0x10000000;
	do
	{
		fgets(buf, 0x100, fd);
		loop_num--;
	} while (!strstr(buf, module) || loop_num == 0);
	if (loop_num == 0)
	{
		printf("search %d lines in mapfile,but don't find %s\n.", 0x10000000, module);
		return 0;
	}
	moduleAddr = strtol(buf, NULL, 16);
	printf("The process %d's module(%s) base is: 0x%lx\n", pid, module, moduleAddr);
	fclose(fd);

	return moduleAddr;
}

typedef struct handle
{
	Elf64_Ehdr *ehdr;
	Elf64_Phdr *phdr;
	Elf64_Shdr *shdr;
	uint8_t *mem;
} handle_t;

/**
 * 根据符号名在 .symtab 查找符号地址
 */
Elf64_Addr lookup_symbol_symtab(handle_t *h, char *symbol, int bind, int type)
{
	unsigned long i, j, NumOfSym;
	char *strtab;
	Elf64_Sym *symtab;

	for (i = 0; i < h->ehdr->e_shnum; i++)
	{
		/* 找 .symtab 节，该节保存了符号信息，每一个符号项为Elf64_Sym */
		if (h->shdr[i].sh_type == SHT_SYMTAB)
		{
			/* 类型为SHT_SYMTAB的节，其sh_link为字符串表所在节表中的下标 */
			/* 因此h->shdr[i].sh_link为字符串表下标 */
			strtab = (char *)(h->mem + h->shdr[h->shdr[i].sh_link].sh_offset);

			/* 获取符号表的首地址 */
			symtab = (Elf64_Sym *)&h->mem[h->shdr[i].sh_offset];

			NumOfSym = h->shdr[i].sh_size / sizeof(Elf64_Sym);

			for (j = 0; j < NumOfSym; j++)
			{
				/* st_name为符号名在字符串表中的下标 */
				char *name = (char *)&strtab[symtab->st_name];
				// printf("symtab->st_info : %d\n", symtab->st_info);
				// printf("symbol name : %s\n", name);
				int name_right = !strcmp(name, symbol);
				int st_info = ELF64_ST_INFO(bind, type);
				// printf("st_info : %d\n", st_info);
				int st_info_right = (st_info == symtab->st_info);
				if (name_right && st_info_right)
				{
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
Elf64_Addr lookup_symbol_dynsym(handle_t *h, char *symbol, int bind, int type)
{
	unsigned long i, j, NumOfSym;
	char *dynstr;
	Elf64_Sym *symtab;

	for (i = 0; i < h->ehdr->e_shnum; i++)
	{
		/* 找 .symtab 节，该节保存了符号信息，每一个符号项为Elf64_Sym */
		if (h->shdr[i].sh_type == SHT_DYNSYM)
		{
			/* 类型为SHT_SYMTAB的节，其sh_link为字符串表所在节表中的下标 */
			/* 因此h->shdr[i].sh_link为字符串表下标 */
			dynstr = (char *)(h->mem + h->shdr[h->shdr[i].sh_link].sh_offset);

			/* 获取符号表的首地址 */
			symtab = (Elf64_Sym *)&h->mem[h->shdr[i].sh_offset];

			NumOfSym = h->shdr[i].sh_size / sizeof(Elf64_Sym);

			for (j = 0; j < NumOfSym; j++)
			{
				/* st_name为符号名在字符串表中的下标 */
				char *name = (char *)&dynstr[symtab->st_name];
				// printf("symtab->st_info : %d\n", symtab->st_info);
				// printf("symbol name : %s\n", name);
				int name_right = !strcmp(name, symbol);
				int st_info = ELF64_ST_INFO(bind, type);
				// printf("st_info : %d\n", st_info);
				int st_info_right = (st_info == symtab->st_info);
				if (name_right && st_info_right)
				{
					/* st_value 为符号的地址 */
					return symtab->st_value;
				}
				symtab++;
			}
		}
	}

	return 0;
}

handle_t init_elf_file(char *path)
{
	handle_t h;
	struct stat st;

	/* 打开指定文件 */
	int fd;
	if ((fd = open(path, O_RDONLY)) < 0)
	{
		perror("open");
		exit(-1);
	}

	/* 获取指定文件的属性 */
	if (fstat(fd, &st) < 0)
	{
		perror("fstat");
		exit(-1);
	}

	/* 将目标文件映射到本进程的虚拟内存中 */
	h.mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (h.mem == MAP_FAILED)
	{
		perror("mmap");
		exit(-1);
	}

	h.ehdr = (Elf64_Ehdr *)h.mem;
	h.phdr = (Elf64_Phdr *)(h.mem + h.ehdr->e_phoff);
	h.shdr = (Elf64_Shdr *)(h.mem + h.ehdr->e_shoff);

	return h;
}

// 解析 ELF 文件 通过遍历 .symtab 或 .dynsym 查找 symbol 对应的 地址
unsigned long offset_symbol(char *symbol, char *path, int bind, int type)
{
	Elf64_Addr offset = 0;
	handle_t h = init_elf_file(path);
	offset = lookup_symbol_symtab(&h, symbol, bind, type);
	if (offset == 0)
	{
		offset = lookup_symbol_dynsym(&h, symbol, bind, type);
	}
	if (offset == 0)
	{
		printf("%s %s %d The %s is not in %s\n", __FILE__, __FUNCTION__, __LINE__, symbol, path);
		exit(-1);
	}

	return (unsigned long)offset;
}

// 计算系统模块(libc)中 symbol 在进程中的虚拟地址
unsigned long get_vaddr_in_system_moudle(pid_t pid, char *symbol, int bind, int type)
{
	char *module = "/system/lib64/libc.so"; // android 系统模块路径

	unsigned long base = get_module_base(pid, module);
	printf("base:           0x%lx\n", base);

	unsigned long offset = offset_symbol(symbol, module, bind, type);
	printf("offset:         0x%lx\n", offset);

	unsigned long vaddr = base + offset; //  模块在目标进程中的基址 加上函数在模块内的偏移 就是函数在目标进程中的虚拟地址
	printf("vaddr:          0x%lx\n", vaddr);

	return vaddr;
}

// 计算指定模块中 symbol 在进程中的虚拟地址
unsigned long get_vaddr(pid_t pid, char *symbol, char *module_path, int bind, int type)
{
	unsigned long base = get_module_base(pid, module_path);
	printf("base:           0x%lx\n", base);

	unsigned long offset = offset_symbol(symbol, module_path, bind, type);
	printf("offset:         0x%lx\n", offset);

	unsigned long vaddr = base + offset; //  模块在目标进程中的基址 加上函数在模块内的偏移 就是函数在目标进程中的虚拟地址
	printf("vaddr:          0x%lx\n", vaddr);

	return vaddr;
}
