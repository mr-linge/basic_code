#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>


int global_var = 10;

int func1(int p1, int p2) {
	//if(global_var == 20) {
	//	__asm__ __volatile__ ("int3");
	//}
	int tmp = global_var * 10;
	return tmp + p1 + p2;
}

int func2(int p1, int p2) {
	printf("Hello, this is func2 ...\n");
	//	__asm__ __volatile__ ("int3");
	return p1 * p2 * global_var;
}

/*
 * gcc tracee.c -o tracee
 * **/
int main() {
	char *str = (char *)malloc(32);
	memset(str,'a',32);

	//char c;
	while(1) {
		printf("****************** pid:%d *******************\n",getpid());
		//printf("input commond:");
		//c = getchar();
		//printf("str addr:%p, value:%s\n",str,str);
		//printf("mmap            addr: %p\n", &mmap);
		printf("&func1          addr: %p\n", &func1);
		printf("&func2          addr: %p\n", &func2);
		global_var += 1;
		//if(global_var == 13) {
		//	func2(6,6);
		//}
		int ret = 0;
		ret = func1(1,2);
		printf("result = %d\n", ret);
		sleep(5);
	}
	return 0;
}
