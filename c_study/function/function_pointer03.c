#include <stdio.h>

void hello() {
	printf("hello world\n");
}


int add(int a, int b)
{
	return a + b;
}

int main() {
	hello();
	printf(" hello = %p\n", hello);
	printf("&hello = %p\n", &hello);
	
	printf("****************%d*********************",__LINE__);

	int(*func1)(int a, int b) = add;
	int ret = func1(1,2);
	printf("func1 result		= %d\n", ret);
	printf(" add addr 		= %p\n", add);
	printf("&add addr 		= %p\n", &add);
	printf(" func1 addr 		= %p\n", func1);
	printf("&func1 addr 		= %p\n", &func1);
	
	return 0;
}
