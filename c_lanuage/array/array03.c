#include <stdio.h>
#include <string.h>

void func1()
{
	char plaintext_text[] = "1234567890abcdef";
	unsigned long len = (unsigned long)(sizeof(plaintext_text) / sizeof(char));
	printf("len = %lu\n", len);
}

void func2()
{
	char plaintext_text[] = "1234567890abcdef";
	unsigned long len = (unsigned long)(strlen(plaintext_text) / sizeof(char));
	printf("len = %lu\n", len);
}

int main()
{
	func1();
	func2();

	return 0;
}
