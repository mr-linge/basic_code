#include <stdio.h>
#include <stdint.h>
#include <string.h>

void func1 () {
	uint8_t plaintext_text[] = "1234567890abcdef";
	unsigned long len = (unsigned long) (sizeof(plaintext_text) / sizeof(uint8_t));
	printf("len = %lu\n", len);

}

void func2 () {
	uint8_t plaintext_text[] = "1234567890abcdef";
	unsigned long len = (unsigned long) (strlen(plaintext_text) / sizeof(uint8_t));
	printf("len = %lu\n", len);
}

void main() {
	func1();
	func2();
}
