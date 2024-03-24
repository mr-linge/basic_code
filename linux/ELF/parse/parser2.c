#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <elf.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>

uint8_t *mem;
char *shstrtab;

Elf64_Ehdr *ehdr;
Elf64_Phdr *phdr;
Elf64_Shdr *shdr;

int main(int argc, char **argv)
{
	int fd;
	struct stat st;

	if (argc < 2)
	{
		printf("Usage: %s <executable>\n", argv[0]);
		exit(0);
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0)
	{
		perror("open");
		exit(-1);
	}

	/* file property, want to get file size. */
	if (fstat(fd, &st) < 0)
	{
		perror("fstat");
		exit(-1);
	}

	/* Map the executable into memory */
	mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (mem == MAP_FAILED)
	{
		perror("mmap");
		exit(-1);
	}

	/*
	 * The initial ELF Header starts at offset 0
	 * of our mapped memory.
	 * */
	ehdr = (Elf64_Ehdr *)mem;

	/*
	 * The phdr table and shdr table offsets are
	 * given by e_phoff and e_shoff members of the Elf64_Ehdr.
	 * */
	phdr = (Elf64_Phdr *)&mem[ehdr->e_phoff];
	shdr = (Elf64_Shdr *)&mem[ehdr->e_shoff];

	/*
	 * Check to see if the ELF magic (The first 4 bytes)
	 * match up as 0x7f ELF.
	 * */
	if (!(*mem == 0x7f && strcmp((char *)(mem + 1), "ELF")))
	{
		fprintf(stderr, "%s is not ELF file\n", argv[1]);
		exit(-1);
	}

	printf("Program Entry point: 0x%lx\n", ehdr->e_entry);

	/*
	 * Print out each segment name, and address.
	 * Except for PT_INTERP we print the path to
	 * the dynamic linker (Interpreter).
	 * */
	printf("\n***********  Program header list  ***********\n\n");
	for (int i = 0; i < ehdr->e_phnum; i++)
	{
		switch (phdr[i].p_type)
		{
		case PT_LOAD:
			/*
			 * We know that text segment starts
			 * at offset 0. And only one other
			 * possible loadable segment exists
			 * which is the data segment.
			 * */
			if (phdr[i].p_offset == 0)
			{
				printf("Text segment: 0x%lx\n", phdr[i].p_vaddr);
			}
			else
			{
				printf("Data segment: 0x%lx\n", phdr[i].p_vaddr);
			}
			break;
		case PT_INTERP:
		{
			char *interp = strdup((char *)&mem[phdr[i].p_offset]);
			printf("Interpreter: %s\n", interp);
		}
		break;
		case PT_NOTE:
			printf("Note segment: 0x%lx\n", phdr[i].p_vaddr);
			break;
		case PT_DYNAMIC:
			printf("Dynamic segment: 0x%lx\n", phdr[i].p_vaddr);
			break;
		case PT_PHDR:
			printf("Phdr segment: 0x%lx\n", phdr[i].p_vaddr);
			break;
		default:
			printf("Unknown segment\n");
			break;
		}
	}

	/*
	 * We find the string table for the section header
	 * names with e_shstrndx which gives the index of
	 * which section holds the string table.
	 * */
	shstrtab = (char *)(mem + shdr[ehdr->e_shstrndx].sh_offset);

	/*
	 * Print each section header name and address.
	 * Notice we get the index into the string table
	 * that contains each section header name with
	 * the shdr.sh_name member.
	 * */
	printf("\n***********  Section header list  ***********\n\n");
	char *strtab;
	Elf64_Sym *symtab;
	int NumOfSym;
	for (int i = 1; i < ehdr->e_shnum; i++)
	{
		printf("%s: sh_addr: 0x%lx\tsh_offset:%lx\n", (char *)(shstrtab + shdr[i].sh_name), shdr[i].sh_addr, shdr[i].sh_offset);
		switch (shdr[i].sh_type)
		{
		case SHT_STRTAB:
			// strtab = (char *) (mem + shdr[i].sh_offset);
			// printf("strtab :%s\n", strtab);
			break;
		case SHT_SYMTAB:
		{
			strtab = (char *)(mem + shdr[shdr[i].sh_link].sh_offset);
			symtab = (Elf64_Sym *)(mem + shdr[i].sh_offset);
			NumOfSym = shdr[i].sh_size / sizeof(Elf64_Sym);
		}
		break;
		}
	}

	printf("\n***********  Symbol list  ***********\n\n");
	printf("NumOfSym = %d\n", NumOfSym);
	for (int j = 0; j < NumOfSym; j++)
	{
		char *name = (char *)(strtab + symtab[j].st_name);
		printf("%2d  name: %s  st_name: %u  st_value: %lu  st_shndx: %u\n", j, name, symtab[j].st_name, symtab[j].st_value, symtab[j].st_shndx);
	}

	return 0;
}
