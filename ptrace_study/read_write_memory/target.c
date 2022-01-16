#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	char *str = (char *)malloc(32);
	memset(str,'a',32);

	while(1) {
		printf("str addr:%p, value:%s\n",str,str);	
		sleep(10);
	}
	return 0;
}
