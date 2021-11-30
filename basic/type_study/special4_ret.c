#include <stdio.h>

void func(int num) {
	num = 20;
	printf("num = %d\n", num);
}

void main() {
	func(30);
}
