#include <stdio.h>
#include <string.h>

int main()
{
	char path[] = "./test/name.txt";
	strcat(path, ".temp");
	char *s2 = "tmp";
	strcat(path, s2);
	printf("%s\n", path);
	
	return 0;
}
