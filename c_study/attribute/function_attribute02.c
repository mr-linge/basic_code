#include <stdio.h>
#include <stdlib.h>

/*
   如果要在main()之前或者是执行完成之后，需要执行很多的前处理动作或者是后处理动作，我们应该怎么处理?

   也许，你需要下面这些东西:

   __attribute__((constructor(PRIORITY)))

   __attribute__((destructor(PRIORITY)))

PRIORITY: 优先级(0-100包括100,是内部保留的，所以在编码的时候需要注意).

注意，编译时加上 -Wall
gcc –Wall xxx.c
 * */

__attribute__((constructor(103))) void pre_proc_1(void) {
	printf("\n%d pre_proc_1\n",__LINE__);
}

__attribute__((destructor(103))) void end_proc_1(void) {
	printf("\n%d end_proc_1\n",__LINE__);
}

__attribute__((constructor(102))) void pre_proc_2(void) {
	printf("\n%d pre_proc_2\n",__LINE__);
}

__attribute__((destructor(102))) void end_proc_2(void) {
	printf("\n%d end_proc_2\n",__LINE__);
}

int main(int argc, char **argv) {
	return EXIT_SUCCESS;
}
