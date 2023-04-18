#include <stdio.h>
#include <stdlib.h>

/*
   __attribute__((constructor)) 在main() 之前执行,__attribute__((destructor)) 在main()执行结束之后执行.

   注意，编译时加上 -Wall
   gcc –Wall xxx.c
 * */
__attribute__((constructor)) void before() {
	printf("%d before()\n",__LINE__);
}

__attribute__((destructor)) void after() {
	printf("%d after()\n",__LINE__);
}

int main(int argc, char** argv) {
	printf(" main\n");
	return EXIT_SUCCESS;
}
