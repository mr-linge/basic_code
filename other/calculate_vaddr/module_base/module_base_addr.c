#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>

// 计算模块在进程中的虚拟地址(即so加载到进程后的首地址)
unsigned long get_module_vaddr(pid_t pid, char *module)
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

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		return -1;
	}
	pid_t pid = atoi(argv[1]);
	char *so_name = argv[2];
	size_t addr = get_module_vaddr(pid, so_name);
	printf("The process %d's (%s) libcbase is: 0x%lx\n", pid, so_name, addr);

	return 0;
}
