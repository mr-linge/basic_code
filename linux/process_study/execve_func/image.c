#include <stdio.h>
#include <unistd.h>

// 此处的environ是一個指針數組，它當中的每一個指針指向的char为"XXX=XXX"
extern char **environ;

int main(int argc, char *argv[])
{
  	printf("%s:%d pid:%d\n", __FILE__, __LINE__, getpid());
	printf("---------------------- argement ----------------------\n");
	printf("argc:%d\n", argc);
	for (int i = 0; i < argc; i++)
	{
		printf("argv[%d]:%s\n", i, argv[i]);
	}

	printf("---------------------- environ ----------------------\n");
	for (int i = 0; environ[i] != NULL; i++)
	{
		printf("environ[%d]:%s\n", i, environ[i]);
	}

  	printf("%s:%d pid:%d\n", __FILE__, __LINE__, getpid());
	return 0;
}
