#include <stdio.h>
#include <unistd.h>

// 此处的environ是一個指針數組，它當中的每一個指針指向的char为"XXX=XXX"
extern char** environ;

/*
   gcc -no-pie target.c -o target.out
   ./target.out param1 param2
 * */
int main(int argc,char * argv[]) {
	printf("file:%s, line:%d, pid:%d\n", __FILE__, __LINE__, getpid());
	printf("argc = %d\n",argc);
	for(int i=0; i < argc; i++){
		printf("argv[%d] = %s\n",i,argv[i]);
	}

	printf("environ :\n");
	for (int i=0; environ[i]!=NULL; i++) {
		printf("%s\n", environ[i]);
	}

	return 0;
}
