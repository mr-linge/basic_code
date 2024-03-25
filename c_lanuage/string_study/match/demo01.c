#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

int main()
{
	char *text = (char *)calloc(16, 1);
	strcpy(text, "0123456789");
	printf("text: %s\n", text);
	char *filling = strrchr(text, '\0');
	printf("filling : %s\n", filling);

	return 0;
}
