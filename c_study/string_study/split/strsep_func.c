#include <stdio.h>
#include <string.h>

/*
 在linux2.6.29以后的版本中，strtok被strsep代替了。
 * */
int main() {
	char str[] = "mv a.c b.c";
	char *p;
	char *buff;
	buff = str;
	p = strsep(&buff, " ");
	printf("buff:%s\n", buff);
	while(p!=NULL) {
		printf("%s\n", p);
		p = strsep(&buff, " ");
	}
	return 0;
}
