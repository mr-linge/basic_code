#include <stdio.h>

long num1 = 0x10;
long num2 = 0x10;
long num3 = 0x20;
long num4 = 0x30;
long num5 = 0;
static long num6 = 0x50;

int add_test(int a,int b) {
	return (a + b);
}

int sub_test(int a, int b) {
	return (a - b);
}

int mul_test(int a, int b) {
	return (a * b);
}

int div_test(int a, int b) {
	return (a / b);
}

static int max_test(int a, int b) {
	return (a > b)? a : b;
}

int main(int argc, char **argv) {
    int a = 0x10, b = 0x20;
    int retval = add_test(a,b);
    printf("add_test(%d, %d) = %d\n",a,b,retval);
    return 0;
}