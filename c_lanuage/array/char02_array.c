#include <stdio.h>
#include <string.h>

int main() {
	char s[] = "aaabbb";
	printf("s size = %lu\n", sizeof(s));
	printf("s len  = %lu\n", strlen(s));
	
	char s2[512];
	char *s2_tmp = "oookkk";
	strcat(s2,s2_tmp);
	printf("s2 size = %lu\n", sizeof(s));
	printf("s2 len  = %lu\n", strlen(s));
	printf("s2:%s\n", s2);

    return 0;
}
