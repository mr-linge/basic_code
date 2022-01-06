#include <stdio.h>

int main() {
	long orig = 0xABCDEF123131;

	// 目标指令的尾部改成自己的指令
	long last_value = 0xcc;
	long trap = (orig & ~0xff) | last_value;
	printf("%lx\n", trap);
	
	return 0;
}
