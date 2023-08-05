#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/mmap.h>

#define PAGE_SHIFT 12
#define PAGE_SIZE (1UL << PAGE_SHIFT)
#define PAGE_MASK (~(PAGE_SIZE - 1))

#define PAGE_START(addr) ((addr)&PAGE_MASK)
#define PAGE_END(addr) (PAGE_START(addr) + PAGE_SIZE)

// 函数指针用于保留原来的执行流程
static int (*old_c_test_func)(int i);

// 新函数
static int new_c_test_func(int i)
{
    int origin_ret = old_c_test_func(i);
    printf("原来的参数:%d,返回值:%d\n", i, origin_ret);
    return 10 * 10;
}

// 计算模块在进程中的虚拟地址(即so加载到进程后的首地址)
unsigned long get_module_vaddr(char *module)
{
    char mapfile[0x20] = {0};
    pid_t pid = getpid();
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

void hook(unsigned long origin_vaddr, unsigned long new_vaddr) {
    int status;
    unsigned long page_start = PAGE_START(origin_vaddr);
	printf("page start : %lx\n", page_start);
	// printf("page end   : %lx\n", page_end);
    status = mprotect((void *)page_start, PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC);  
	if (status < 0)
	{
		perror("mprotect err");
		return;
	}

     unsigned  char jumpCode[4] = {0x2d, 0x00, 0x00, 0x14};
     memcpy(originAddr, jumpCode, 4);

    status = mprotect((void *)page_start, PAGE_SIZE, PROT_EXEC);  
	if (status < 0)
	{
		perror("mprotect err");
		return;
	}
}

void __attribute__((constructor)) dylibInject(void)
{
    printf("Hello, hook was starting ...\n");
    char *module_name = "/data/local/tmp/main";
    unsigned long module_vaddr = get_module_vaddr(module_name);
    printf("%s --> %s vaddr:0x%lx\n", __FUNCTION__, module_name, module_vaddr);

    unsigned long target_func_vaddr = module_vaddr + 0x18bc;

    hook(target_func_vaddr,(unsigned long)&new_c_test_func);
}
