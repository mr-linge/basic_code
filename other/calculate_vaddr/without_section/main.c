#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <dlfcn.h>

// 计算模块在进程中的虚拟地址(即so加载到进程后的首地址)
size_t get_module_base(int pid, char *module)
{
	char mapfile[0x20] = {0};
	sprintf(mapfile, "/proc/%d/maps", pid);
	FILE *fd = fopen(mapfile, "r");
	if (!fd)
	{
		printf("open %s error!\n", mapfile);
		fclose(fd);
		return 0;
	}
	char buf[0x400] = {0};
	// 按行依次读取文件所有内容
	while (fgets(buf, 0x400, fd) != NULL)
	{
		if (strstr(buf, module) != NULL)
		{
			break;
		}
	}

	unsigned long module_vaddr = strtol(buf, NULL, 16);
	// printf("The process %d's module(%s) base is: 0x%lx\n", pid, module, module_vaddr);
	fclose(fd);

	return module_vaddr;
}

// 计算 函数 在模块中的偏移(模块必需在当前进程中，也即动态库xxx.so 被当前进程加载进来了。否则在当前进程中则找不到这个模块)
size_t get_func_offset(char *funcName, char *moduleName)
{
	// get function offset from self process, the shared libc.so
	size_t funcAddr = (size_t)dlsym(0, funcName); // 获取当前进程中 名为funcName 的函数 所在的 虚拟地址 vaddr
	size_t moduleAddr = get_module_base(getpid(), moduleName);
	size_t offset = funcAddr - moduleAddr; // 当前函数地址 减去 函数所在模块(即加载的libc-2.31.so)在进程中的虚拟地址 即得到 函数在模块内的偏移
	printf("function:%s in module:%s\n", funcName, moduleName);
	printf("funcAddr: 		0x%lx\n", funcAddr);
	printf("moduleAddr: 		0x%lx\n", moduleAddr);
	printf("offset: 		0x%lx\n", offset);

	return offset;
}

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
		exit(1);
	}

	pid_t target_pid = atoi(argv[1]);
	char *funcName = argv[2];
	// printf("funcName : %s\n", funcName);
	char *moduleName = argv[3]; // "libc-";

	size_t base = get_module_base(target_pid, moduleName);
	printf("base:	 	0x%lx\n", base);

	size_t offset = get_func_offset(funcName, moduleName);
	printf("offset:		0x%lx\n", offset);

	size_t vaddr = base + offset; //  模块在目标进程中的基址 加上函数在模块内的偏移 就是函数在目标进程中的虚拟地址
	printf("vaddr:		0x%lx\n", vaddr);

	return 0;
}
