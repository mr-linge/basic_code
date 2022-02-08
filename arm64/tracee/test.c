#include <stdio.h>

void exception() {
	//__asm__ volatile ("BKPT");
	__builtin_trap();
}

int main() {
	exception();	
	return 0;
}
