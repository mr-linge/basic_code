#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <dlfcn.h>

long global_var = 0x1;

int func0()
{
	printf("Hello, this is func0 ...\n");
	return 0xFF00;
}

int func1(int p1)
{
	printf("Hello, this is func1 ...\n");
	return global_var + p1;
}

int func2(int p1, int p2)
{
	printf("Hello, this is func2 ...\n");
	return p1 * p2;
}

char *func3(char *str)
{
	printf("<%s %d> origin param string: %s\n", __FUNCTION__, __LINE__, str);
	char *ret = "You will be success!";
	printf("<%s %d> param vaddr: %p, ret :%p\n", __FUNCTION__, __LINE__, str, ret);
	char tmp[0x100];
	memset(tmp,0,0x100);
	memcpy(tmp,str,strlen(str));
	printf("<%s %d> tmp string: %s, vaddr:%p\n", __FUNCTION__, __LINE__, tmp, &(tmp[0]));
	return ret;
}

void read_mem(unsigned long vaddr, unsigned long len)
{
	printf("Hello, this is read_mem ...\n");
	uint8_t *tmp_addr = (uint8_t *)vaddr;
	for (unsigned long i = 0; i < len; i++)
	{
		printf("%02x ", *(tmp_addr + i));
	}
	puts("");
}

int func10(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10)
{
	printf("Hello, this is func10 ...\n");
	printf("p1 = %x,p2 = %x,p3 = %x,p4 = %x,p5 = %x,p6 = %x,p7 = %x,p8 = %x,p9 = %x,p10 = %x\n",
		   p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	return p1 + p10;
}

int func20(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14, int p15, int p16, int p17, int p18, int p19, int p20)
{
	printf("<%s> p1 = %x,p2 = %x,p3 = %x,p4 = %x,p5 = %x,p6 = %x,p7 = %x,p8 = %x,p9 = %x,p10 = %x,p11 = %x,p12 = %x,p13 = %x,p14 = %x,p15 = %x,p16 = %x,p17 = %x,p18 = %x,p19 = %x,p20 = %x\n",
		   __FUNCTION__, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20);
	return p1 + p20;
}

int func21(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14, int p15, int p16, int p17, int p18, int p19, int p20)
{
	printf("<%s> p1 = %x,p2 = %x,p3 = %x,p4 = %x,p5 = %x,p6 = %x,p7 = %x,p8 = %x,p9 = %x,p10 = %x,p11 = %x,p12 = %x,p13 = %x,p14 = %x,p15 = %x,p16 = %x,p17 = %x,p18 = %x,p19 = %x,p20 = %x\n",
		   __FUNCTION__, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20);
	return p1 * p20;
}

void virtual_stack_100(uint8_t *stack_bottom, int len)
{
	printf("virtual stack: 0x%p\n", stack_bottom);
	for (int i = 0; i < len; i++)
	{
		printf("%02x ", *(stack_bottom + i));
	}
	puts("");
}

char *path = "/home/dio/Repositories/C_study/ptrace_study/tracee/libcaculate.so";
void print(char *str)
{
	puts(str);
}

int main()
{
	printf("&func1           addr: %p\n", &func1);
	printf("&func2           addr: %p\n", &func2);
	printf("&func3           addr: %p\n", &func3);
	printf("&func10          addr: %p\n", &func10);
	printf("&func20          addr: %p\n", &func20);
	printf("&func21          addr: %p\n", &func21);
	puts("输入控制指令\n1:调用func1\n2:调用func2\n......\n999:退出程序.");

	int len = 64;
	uint8_t *virtual_stack = (uint8_t *)malloc(len);
	memset(virtual_stack, 'a', 32);
	//	int fd = open("./loaded_file", O_RDONLY | O_WRONLY);
	//打开动态链接库
	//	void *handle = dlopen(path, RTLD_LAZY);
	//	printf("handle = %p\n",handle);
	// char c;
	// while(1) {
	// 	printf("****************** pid:%d *******************\n",getpid());
	// 	//printf("input commond:");
	// 	//c = getchar();
	// 	printf("mem_space addr:%p,value:\n",mem_space);
	// 	for(int i = 0; i< len; i++) {
	// 		printf("%02x ", *(mem_space + i));
	// 	}
	// 	puts("");
	// 	//		printf("fd = %d\n", fd);
	// 	//read_mem((unsigned long)mem_space,len);
	// 	//		printf("open            addr: %p\n", &open);
	// 	//printf("mmap            addr: %p\n", &mmap);
	// 	//printf("dlopen          addr: %p\n", &dlopen);
	// 	printf("printf          addr: %p\n", &printf);
	// 	//printf("&func0          addr: %p\n", &func0);
	// 	printf("&global_var     addr: %p\n", &global_var);
	// 	printf("&func1          addr: %p\n", &func1);
	// 	printf("&func2          addr: %p\n", &func2);
	// 	//printf("&read_mem       addr: %p\n", &read_mem);
	// 	//printf(" path           addr: %p\n", path);
	// 	//printf("&print          addr: %p\n", &print);
	// 	printf("&func10         addr: %p\n", &func10);
	// 	printf("&func20         addr: %p\n", &func20);
	// 	global_var++;
	// 	int ret = 0;
	// 	//ret = func1(1,1);
	// 	//ret = func10(1,2,3,4,5,6,7,8,9,10);
	// 	ret = func20(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20);
	// 	printf("result = 0x%x\n", ret);
	// 	sleep(5);
	// }

	int status = 0;
	while (1)
	{
		printf("****************** pid:%d *******************\n", getpid());
		printf("input:");
		scanf("%d", &status);
		switch (status)
		{
		case 1:
		{
			int result = func1(10);
			printf("<%s> func1 return value : 0x%x\n", __FUNCTION__, result);
			break;
		}
		case 3:
		{
			char *ret = func3("Good luck are for you!");
			printf("<%s %d>return value: %s\n", __FUNCTION__, __LINE__, ret);
			break;
		}
		case 20:
		{
			int result = func20(0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20);
			printf("<%s> func20 return value : 0x%x\n", __FUNCTION__, result);
			break;
		}
		case 21:
		{
			int result = func21(0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x100, 0x111, 0x122, 0x133, 0x144, 0x155, 0x166, 0x177, 0x188, 0x199, 0x200);
			printf("<%s> func21 return value : 0x%x\n", __FUNCTION__, result);
			break;
		}
		case 100:
		{
			virtual_stack_100(virtual_stack, len);
			break;
		}
		case 999:
			goto done;
		default:
			puts("输入命令不合法");
			break;
		}
	}

done:
	puts("program is over!");

	return 0;
}
