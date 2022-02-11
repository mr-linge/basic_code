#include <stdio.h>

// 根据地址调用 printf() 这种形式的函数
typedef int(*MY_PRINTF)(const char *__restrict __format, ...);

int init_symbol(void *printf_vaddr) {
	MY_PRINTF my_print = 0;
	my_print = (MY_PRINTF) printf_vaddr;
	char *str = "Hello world! init_symbol";
	my_print("%s\n", str);
	return 0;
}

int main() {
	void * printf_addr = &printf;
	init_symbol(printf_addr);
	return 0;
}
