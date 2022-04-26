#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <linux/limits.h>

#include <fcntl.h>
#include <elf.h>

#include <sys/stat.h>


// 计算模块在进程中的虚拟地址(即so加载到进程后的首地址)
unsigned long get_module_base(pid_t pid, char *moduleName);

// 解析 ELF 文件 通过遍历 .symtab 或 .dynsym 查找 symbol 对应的 地址
unsigned long offset_symbol(char *symbol,char *path,int bind,int type);

// 计算系统模块(libc)中 symbol 在进程中的虚拟地址
unsigned long get_vaddr_in_system_moudle(pid_t pid,char *symbol,int bind,int type);
// 计算指定模块中 symbol 在进程中的虚拟地址
unsigned long get_vaddr(pid_t pid,char *symbol,char *module_path,int bind,int type);
