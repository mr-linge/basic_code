#include <stdio.h>
#include <string.h>

/*
 在linux2.6.29以后的版本中，strtok被strsep代替了。
 * */
int main()
{
	char p0[] = "mv a.c b.c";
	char *p1 = p0;
	char *p2 = strsep(&p1, " ");
	printf("p0:%s\n", p0);
	printf("p1:%s\n", p1);
	printf("p2:%s\n", p2);

	while (p2 != NULL)
	{
		p2 = strsep(&p1, " ");
		printf("p2:%s\n", p2);
	}
	return 0;
}
