#include <stdio.h>

int main(){
	int num = 0;
loop:
	num++;
	printf("loop num = %d\n",num);

	if (num < 10) {
		printf("if block num = %d\n",num);
		goto loop;
	}
	return 0;
}
