#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdint.h>

long func10(long p1, long p2, long p3, long p4, long p5, long p6, long p7, long p8, long p9, long p10)
{
	long a1 = 0x10;
	long a2 = 0x11;
	long a3 = 0x12;
	long a4 = 0x13;
	printf("p1 = 0x%lx,p2 = 0x%lx,p3 = 0x%lx,p4 = 0x%lx,p5 = 0x%lx,p6 = 0x%lx,p7 = 0x%lx,p8 = 0x%lx,p9 = 0x%lx,p10 = 0x%lx\n",
		   p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	long tmp = a1 + a2 + a3 + a3;
	return tmp + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10;
}

int main()
{
	long p1 = 0x1;
	long p2 = 0x2;
	long p3 = 0x3;
	long p4 = 0x4;
	long p5 = 0x5;
	long p6 = 0x6;
	long p7 = 0x7;
	long p8 = 0x8;
	long p9 = 0x9;
	long p10 = 0x10;

	while (1)
	{
		printf("****************** pid:%d *******************\n", getpid());
		printf("&func10         addr: %p\n", &func10);
		long ret = func10(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
		printf("ret = 0x%lx\n", ret);
		sleep(5);
	}

	return 0;
}
