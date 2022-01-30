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

Elf64_Ehdr *ehdr;
Elf64_Phdr *phdr;
Elf64_Shdr *shdr;
uint8_t *mem;

// 描述 ELF Header
void descrite_ELF_header();
// 描述 Program Header
void descrite_Program_header();

int main(int argc, char ** argv) {
	int fd;
	struct stat st;
	char *StringTable;

	if(argc < 2) {
		printf("Usage: %s <executable>\n",argv[0]);
		exit(0);
	}

	if((fd = open(argv[1], O_RDONLY)) < 0) {
		perror("open");
		exit(-1);
	}

	/* file property, want to get file size. */
	if(fstat(fd, &st) < 0) {
		perror("fstat");
		exit(-1);
	}

	/* Map the executable into memory */
	mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(mem == MAP_FAILED) {
		perror("mmap");
		exit(-1);
	}

	/*
	 * Check to see if the ELF magic (The first 4 bytes)
	 * match up as 0x7f ELF.
	 * */
	if(!(mem[0] == 0x7f && strcmp((char *)&mem[1], "ELF"))) {
		fprintf(stderr, "%s is not ELF file\n", argv[1]);
		exit(-1);
	}

	/*
	 * The initial ELF Header starts at offset 0
	 * of our mapped memory.
	 * */
	ehdr = (Elf64_Ehdr *)mem;
	// printf("ELF header start address(vaddr): %p\n", ehdr);

	/*
	 * We are only parsing executales with this code.
	 * so ET_EXEC marks an executable.
	 * */
	if(ehdr->e_type != ET_EXEC) {
		fprintf(stderr, "%s is not an executable\n", argv[1]);
		exit(-1);
	}

	printf("\nProgram Entry point: 0x%lx\n", ehdr->e_entry);
	descrite_ELF_header(ehdr);

	/*
	 * The phdr table and shdr table offsets are
	 * given by e_phoff and e_shoff members of the Elf64_Ehdr.
	 * */
	phdr = (Elf64_Phdr *)&mem[ehdr->e_phoff];
	shdr = (Elf64_Shdr *)&mem[ehdr->e_shoff];

	descrite_Program_header(ehdr, phdr);


	/* 
	 * We find the string table for the section header
	 * names with e_shstrndx which gives the index of
	 * which section holds the string table.
	 * */
	StringTable = &mem[shdr[ehdr->e_shstrndx].sh_offset];

	/*
	 * Print each section header name and address.
	 * Notice we get the index into the string table
	 * that contains each section header name with
	 * the shdr.sh_name member.
	 * */
	printf("\n***********  Section header list  ***********\n\n");
	for(int i = 1; i < ehdr->e_shnum; i++){
		printf("%s: 0x%lx\n", &StringTable[shdr[i].sh_name], shdr[i].sh_addr);
	}

	return 0;
}


void descrite_ELF_header() {
	printf("\n***********  ELF header  ***********\n\n");
	for(int i = 0; i < EI_NIDENT; i++) {
		printf("e_ident[%x]:%02x\t", i,  ehdr->e_ident[i]);
	}
	printf("\n");
	printf("e_type  	: %04x\n", ehdr->e_type);
	printf("e_machine 	: %04x\n", ehdr->e_machine);
	printf("e_version 	: %08x\n", ehdr->e_version);
	printf("e_entry 	: %016lx\n", ehdr->e_entry);
	printf("e_phoff 	: %016lx\n", ehdr->e_phoff);
	printf("e_shoff 	: %016lx\n", ehdr->e_shoff);
	printf("e_flags 	: %08x\n", ehdr->e_flags);
	printf("e_ehsize 	: %04x\n", ehdr->e_ehsize);
	printf("e_phentsize 	: %04x\n", ehdr->e_phentsize);
	printf("e_phnum 	: %04x\n", ehdr->e_phnum);
	printf("e_shentsize 	: %04x\n", ehdr->e_shentsize);
	printf("e_shnum 	: %04x\n", ehdr->e_shnum);
	printf("e_shstrndx 	: %04x\n", ehdr->e_shstrndx);
}


void descrite_Program_header() {
	/*
	 * Print out each segment name, and address.
	 * Except for PT_INTERP we print the path to
	 * the dynamic linker (Interpreter).
	 * */
	printf("\n***********  Program header list  ***********\n\n");
	char * segment_type;
	for(int i = 0; i < ehdr->e_phnum; i++) {
		switch(phdr[i].p_type) {
			case PT_NULL:
				segment_type = "null";
				break;
			case PT_LOAD:
				/*
				 * We know that text segment starts
				 * at offset 0. And only one other
				 * possible loadable segment exists
				 * which is the data segment.
				 * */
				if(phdr[i].p_offset == 0) {
					segment_type = "Text";
				} else {
					segment_type = "Data";
				}
				break;
			case PT_DYNAMIC:
				segment_type = "Dynamic";
				break;
			case PT_INTERP:
				segment_type = "Interpreter";
				char *interp = strdup((char *)&mem[phdr[i].p_offset]);
				printf("Interpreter: %s\n", interp);
				break;
			case PT_NOTE:
				segment_type = "Note";
				break;
			case PT_SHLIB:
				segment_type = "SHLIB";
				break;
			case PT_PHDR:
				segment_type = "PHDR";
				break;
			case PT_LOPROC:
				segment_type = "LOPROC";
				break;
			case PT_HIPROC:
				segment_type = "HIPROC";
				break;
			case PT_GNU_STACK:
				segment_type = "GNU_STACK";
				break;
			case PT_GNU_EH_FRAME:
				segment_type = "GNU_EH_FRAME";
				break;
			case PT_GNU_RELRO:
				segment_type = "GNU_RELRO";
				break;
			default:
				segment_type = "NOT_Support";
				break;

		}
		printf("%s segment\n p_type: %x, p_offset: %lx, p_vaddr: %lx, p_paddr: %lx, p_filesz: %lx, p_memsz: %lx, p_flags: %x, p_align: %lx\n", segment_type, phdr[i].p_type, phdr[i].p_offset, phdr[i].p_vaddr, phdr[i].p_paddr, phdr[i].p_filesz, phdr[i].p_memsz, phdr[i].p_flags, phdr[i].p_align);
	}
}

