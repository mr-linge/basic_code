#include <stdio.h>
#include <unistd.h>

// 此处的environ是一個指針數組,它当中的每一個指針指向的char为"XXX=XXX"
extern char **environ;

int main(int argc, char *argv[])
{
	printf("file:%s, line:%d, pid:%d\n", __FILE__, __LINE__, getpid());
	printf("---------------------- argement ----------------------\n");
	printf("%s:%d argc:%d\n",__FILE__, __LINE__, argc);
	for (int i = 0; i < argc; i++)
	{
		printf("%s:%d argv[%d]:%s\n",__FILE__, __LINE__, i, argv[i]);
	}

	printf("---------------------- environ ----------------------\n");
	for (int i = 0; environ[i] != NULL; i++)
	{
		printf("%s:%d environ[%d]:%s\n",__FILE__, __LINE__, i, environ[i]);
	}

	printf("file:%s, line:%d, pid:%d\n",__FILE__, __LINE__, getpid());
	return 0;
}
