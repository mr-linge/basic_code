#include <stdio.h>

#include "my.h"

int global_var = 10;

int func1(int p1, int p2) {
	return p1 + p2;
}

int main() {
	//printf("&global_var:			%p\n",(void *)&global_var);
	//printf("&func1:				%p\n",(void *)&func1);
	//printf("&main:				%p\n",(void *)&main);
	//printf("&my_global_var1:		%p\n",(void *)&my_global_var1);
//	printf("&math_global_var2 :		%p\n",(void *)&math_global_var2);

	int tmp1 = func1(10,20);
	int tmp2 = max(tmp1,15);
	printf("tmp2 = %d\n", tmp2);
	return 0;
}
