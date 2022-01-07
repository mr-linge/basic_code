//To compile: gcc -fpic -pie -nostdlib payload.c -o payload
#include <stdio.h>
#include <asm/unistd_64.h>
long _write(long fd, char *buf, unsigned long len){
	long ret;
	__asm__ volatile(       
			"mov %0, %%rdi\n"
			"mov %1, %%rsi\n"  
			"mov %2, %%rdx\n"  
			"mov $1, %%rax\n"
			"syscall" : : "g"(fd), "g"(buf), "g"(len));
	asm("mov %%rax, %0" : "=r"(ret));
	return ret;                   
}                                     

void Exit(long status){
	__asm__ volatile("mov %0, %%rdi\n"
			"mov $60, %%rax\n"
			"syscall" : : "r"(status));
}

int _start(){
	_write(1, "I am payload who has hijacked your process!\n", 48);
	//    _write(1, "123456\n", 8);
	Exit(0);
}
