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

int func0(){
	printf("Hello, this is func0 ...\n");
	return 0xFF00;
}

int func1(int p1, int p2) {
	printf("Hello, this is func1 ...\n");
	return global_var + p1 + p2;
}

int func2(int p1, int p2) {
	printf("Hello, this is func2 ...\n");
	return p1 * p2;
}

void read_mem(unsigned long vaddr,unsigned long len) {
	printf("Hello, this is read_mem ...\n");
	uint8_t * tmp_addr = (uint8_t *)vaddr;
	for(unsigned long i = 0; i < len; i++) {
		printf("%02x ",*(tmp_addr + i));
	}
	puts("");
}

int func10(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10) {
	printf("Hello, this is func10 ...\n");
	printf("p1 = %x,p2 = %x,p3 = %x,p4 = %x,p5 = %x,p6 = %x,p7 = %x,p8 = %x,p9 = %x,p10 = %x\n",
			p1,p2,p3,p4,p5,p6,p7,p8,p9,p10);
	return p1+p10;
}

int func20(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10,int p11, int p12, int p13, int p14, int p15, int p16, int p17, int p18, int p19, int p20) {
	printf("Hello, this is %s ...\n",__FUNCTION__);
	printf("p1 = %x,p2 = %x,p3 = %x,p4 = %x,p5 = %x,p6 = %x,p7 = %x,p8 = %x,p9 = %x,p10 = %x,p11 = %x,p12 = %x,p13 = %x,p14 = %x,p15 = %x,p16 = %x,p17 = %x,p18 = %x,p19 = %x,p20 = %x\n",
			p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20);
	return p1+p20;
}

char *path = "/home/dio/Repositories/C_study/ptrace_study/tracee/libcaculate.so";
void print(char *str) {
	puts(str);
}

void sighandler(int);

/*
 * gcc -no-pie tracee.c -o tracee
 * **/
int main() {
	//	signal(SIGINT, sighandler);

	const int len = 64;
	uint8_t *mem_space = (uint8_t *)malloc(len);
	memset(mem_space,'a',32);
	//	int fd = open("./loaded_file", O_RDONLY | O_WRONLY);
	//打开动态链接库
	//	void *handle = dlopen(path, RTLD_LAZY);
	//	printf("handle = %p\n",handle);
	//char c;
	while(1) {
		printf("****************** pid:%d *******************\n",getpid());
		//printf("input commond:");
		//c = getchar();
		printf("mem_space addr:%p,value:\n",mem_space);
		for(int i = 0; i< len; i++) {
			printf("%02x ", *(mem_space + i));
		}
		puts("");
		//		printf("fd = %d\n", fd);
		//read_mem((unsigned long)mem_space,len);
		//		printf("open            addr: %p\n", &open);
		//printf("mmap            addr: %p\n", &mmap);
		//printf("dlopen          addr: %p\n", &dlopen);
		printf("printf          addr: %p\n", &printf);
		//printf("&func0          addr: %p\n", &func0);
		printf("&global_var     addr: %p\n", &global_var);
		printf("&func1          addr: %p\n", &func1);
		printf("&func2          addr: %p\n", &func2);
		//printf("&read_mem       addr: %p\n", &read_mem);
		//printf(" path           addr: %p\n", path);
		//printf("&print          addr: %p\n", &print);
		printf("&func10         addr: %p\n", &func10);
		printf("&func20         addr: %p\n", &func20);
		global_var++;
		int ret = 0;
		//ret = func1(1,1);
		ret = func10(1,2,3,4,5,6,7,8,9,10);
		//ret = func20(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20);
		printf("result = 0x%x\n", ret);
		sleep(5);
	}

	return 0;
}

void sighandler(int signum) {
	printf("signal %d, exit ...\n", signum);
	exit(1);
}
