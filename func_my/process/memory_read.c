#include <stdio.h>
#include <sys/ptrace.h>
#include <string.h>

void pid_read(int pid, void * dst, const void * src, size_t len) {
	int sz = len / sizeof(void *);
	unsigned long *s = (unsigned long *)src;
	long *d = (unsigned long *)dst;
	long word;
	while(sz != 0) {
		word = ptrace(PTRACE_PEEKTEXT, pid, s, NULL);
		*d = word;
		s++;
		d++;
		sz--;
	}
}

int main(int argc, char * argv[]) {
	if(argc < 2) {
		 printf("The paramers is less then 2.\n");
		 return -1;
	}
	int pid = aoti(argv[1]);
	char *dst;

	return 0;
}
