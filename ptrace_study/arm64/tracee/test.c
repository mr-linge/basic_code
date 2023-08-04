#include <stdio.h>

int func10(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10) {
        printf("Hello, this is func10 ...\n");
        printf("p1 = %d,p2 = %d,p3 = %d,p4 = %d,p5 = %d,p6 = %d,p7 = %d,p8 = %d,p9 = %d,p10 = %d\n",
                        p1,p2,p3,p4,p5,p6,p7,p8,p9,p10);
        return p1+p10;
}

int main() {
	int p1 = 1;
	int p2 = 2;
	int p3 = 3;
	int p4 = 4;
	int p5 = 5;
	int p6 = 6;
	int p7 = 7;
	int p8 = 8;
	int p9 = 9;
	int p10 = 10;
	int ret = func10(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10);
	return 0;
}
