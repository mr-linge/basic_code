#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdint.h>


int global_var = 0x1;

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
	return p1 * p2 * global_var;
}

int func10(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10) {
	printf("Hello, this is func10 ...\n");
	printf("p1 = %d,p2 = %d,p3 = %d,p4 = %d,p5 = %d,p6 = %d,p7 = %d,p8 = %d,p9 = %d,p10 = %d\n",
			p1,p2,p3,p4,p5,p6,p7,p8,p9,p10);
	return p1+p10;
}


void sighandler(int);

/*
 * gcc -no-pie tracee.c -o tracee
 * **/
int main() {
	signal(SIGINT, sighandler);
	
	int p0 = 100;
        int p1 = 200;
        int p2 = 300;
        int p3 = 400;
        int p4 = 500;
        int p5 = 600;
        int p6 = 700;
        int p7 = 800;
        int p8 = 900;
        int p9 = 901;
        int ret = func10(p0,p1,p2,p3,p4,p5,p6,p7,p8,p8);
	printf("ret = %d\n",ret);

	return 0;
}

void sighandler(int signum) {
	printf("signal %d, exit ...\n", signum);
	exit(1);
}

