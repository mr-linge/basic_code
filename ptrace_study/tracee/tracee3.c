#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdint.h>

int func10(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10)
{
	return 1;
}

int func20(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14, int p15, int p16, int p17, int p18, int p19, int p20)
{
	return p1 + p20;
}

int main()
{
	int p1 = 0x1;
	int p2 = 0x2;
	int p3 = 0x3;
	int p4 = 0x4;
	int p5 = 0x5;
	int p6 = 0x6;
	int p7 = 0x7;
	int p8 = 0x8;
	int p9 = 0x9;
	int p10 = 0x10;
	int p11 = 0x11;
	int p12 = 0x12;
	int p13 = 0x13;
	int p14 = 0x14;
	int p15 = 0x15;
	int p16 = 0x16;
	int p17 = 0x17;
	int p18 = 0x18;
	int p19 = 0x19;
	int p20 = 0x20;

	// func10(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10);
	func20(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20);
	return 0;
}
