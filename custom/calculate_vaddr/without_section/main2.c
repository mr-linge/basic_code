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
unsigned long get_func_offset(char *funcName, char *moduleName, void *handle)
{
	// get function offset from self process, the shared libc.so
	unsigned long funcAddr = (size_t)dlsym(handle, funcName); // 获取当前进程中 名为funcName 的函数 所在的 虚拟地址 vaddr
	unsigned long moduleAddr = get_module_base(getpid(), moduleName);
	unsigned long offset = funcAddr - moduleAddr; // 当前函数地址 减去 函数所在模块(即加载的libc-2.31.so)在进程中的虚拟地址 即得到 函数在模块内的偏移
	printf("function:%s in module:%s\n", funcName, moduleName);
	printf("funcAddr: 		0x%lx\n", funcAddr);
	printf("moduleAddr: 		0x%lx\n", moduleAddr);
	printf("offset: 		0x%lx\n", offset);

	return offset;
}

void *load_module(char *lib_path)
{
	void *handle;
	// 打开动态链接库
	handle = dlopen(lib_path, RTLD_LAZY);
	// printf("handle = %p\n",handle);
	if (!handle)
	{
		fprintf(stderr, "%s\n", dlerror());
		exit(-1);
	}

	// 清除之前存在的错误
	dlerror();

	// 关闭动态链接库,关闭后该动态库就会从 当前进程中 移除
	//	dlclose(handle);

	return handle;
}

int main(int argc, char *argv[])
{
	printf("current pid : %d\n", getpid());

	char *lib_path = "./libinject.so";

	void *handle = load_module(lib_path);

	puts("*********************************");

	get_func_offset("func2", "libinject.so", handle);
	get_func_offset("mmap", "libc-", NULL);

	dlclose(handle);

	return 0;
}
