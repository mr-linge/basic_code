#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <dlfcn.h>

// 计算模块在进程中的虚拟地址(即so加载到进程后的首地址)
size_t get_module_base(int pid, char *moduleName)
{
	size_t moduleAddr;
	char* buf;
	char* end;
	char mapfile[0x18];
	sprintf(mapfile, "/proc/%d/maps", pid);
	FILE* fd = fopen(mapfile, "r");
	if(!fd)
	{
		printf("open maps error!");
		exit(1);
	}
	//search the libc-.....
	buf = (char*) malloc(0x100);
	do{
		fgets(buf, 0x100, fd);
	} while(!strstr(buf, moduleName));
	end = strchr(buf, '-');
	moduleAddr = strtol(buf, &end, 16);
	printf("The process %d's module base is: 0x%lx\n", pid, moduleAddr);
	fclose(fd);

	return moduleAddr;
}

typedef void *(*MY_MMAP)(void *start, size_t length, int prot, int flags, int fd, off_t offsize);

// 计算 函数 在模块中的偏移
size_t get_func_offset(int pid,char *funcName,char *moduleName)
{
	//get function offset from self process, the shared libc.so
	MY_MMAP funcAddr = (MY_MMAP)dlsym(0, "mmap"); // 获取当前进程中 名为funcName 的函数 所在的 虚拟地址 vaddr
	size_t moduleAddr = get_module_base(getpid(),moduleName);
	size_t offset     = (size_t)funcAddr - moduleAddr; // 当前函数地址 减去 函数所在模块(即加载的libc-2.31.so)在进程中的虚拟地址 即得到 函数在模块内的偏移(仅适用于ARM64,x86的计算方法不同)
	printf("function:%s in module:%s\n",funcName,moduleName);
	printf("funcAddr: 		0x%lx\n",(size_t)funcAddr);
	printf("moduleAddr: 		0x%lx\n",moduleAddr);
	printf("offset: 		0x%lx\n",offset);

	return offset;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
		exit(1);
	}

	pid_t target_pid = atoi(argv[1]);
	char * funcName = argv[2];
	printf("funcName : %s\n", funcName);
	
	char *moduleName = "libc-";
	size_t base = get_module_base(target_pid,moduleName);
	printf("base:	 	0x%lx\n", base);

	size_t offset = get_func_offset(target_pid,funcName,moduleName);
	printf("offset:		0x%lx\n", offset);

	size_t vaddr = base + offset; //  模块在目标进程中的基址 加上函数在模块内的偏移 就是函数在目标进程中的虚拟地址 
	printf("vaddr:		0x%lx\n", vaddr);

	return 0;
}

