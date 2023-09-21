#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <dlfcn.h>

// 计算模块虚拟地址
unsigned long get_module_vaddr(pid_t pid, char *module);
// 获取 symbol 偏移
unsigned long get_symbol_offset(char *module, char *symbol, int type);
