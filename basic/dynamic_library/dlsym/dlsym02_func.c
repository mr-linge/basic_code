#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <stdint.h>

/***************
 * dlsym 获取 local symbol 返回值是NULL,可以看出dlsym 只能获取 xxx.so 里的symbol 不能获取本地 image 里的 symbol.
 * ******************/

typedef int (*CAC_FUNC)(int, int);

long test_num = 0x999;

int test_func(int a, int b) {
	return a + b;
}

/**
 * gcc dlsym02_func.c -o main -ldl
 * **/
int main() {
	printf("test_num origin addr:%p, value:%lx\n", &test_num,test_num);
	size_t test_num_addr  = (size_t)dlsym(0,"test_num");
	printf("test_num dlsym  addr:0x%lx\n", test_num_addr);
	
	printf("test_func origin addr:%p\n", &test_func);
	CAC_FUNC test_func_dlsym = dlsym(0, "test_func");
	printf("test_func dlsym  addr:0x%lx\n", (size_t)test_func_dlsym);
	
	return 0;
}
