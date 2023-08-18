#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>

#define PAGESIZE 0x1000
#define PAGE_START(addr) ((addr) & (~(PAGESIZE - 1)))

union Register
{
    unsigned char bytes[8];
    unsigned long val;
};

unsigned long get_module_vaddr(char *module);

void hook(void *target_vaddr, void *new_vaddr, void **old_vaddr);
