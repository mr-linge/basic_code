#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


int global_var = 10;

int func1(int p1, int p2) {
	int tmp = global_var * 10;
	return tmp + p1 + p2;
}

int main() {
	char *str = (char *)malloc(32);
	memset(str,'a',32);

	while(1) {
		printf("pid:%d, str addr:%p, value:%s\n",getpid(),str,str);
		global_var += 1;
		int ret = func1(1,2);
		printf("result = %d\n", ret);
		sleep(10);
	}
	return 0;
}
