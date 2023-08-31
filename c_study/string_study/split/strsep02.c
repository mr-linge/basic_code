#include <stdio.h>
#include <string.h>
#include <stdlib.h>

union Register
{
	unsigned char bytes[8];
	unsigned long val;
};

int main()
{
	char *tmp = (char *)malloc(0x100);
	memset(tmp, '\0', 0x100);
	scanf("%s", tmp);
	char *param1 = strsep(&tmp, "|");

	printf("%d %s\n", __LINE__, param1);
	// memcpy(tmp, param1, strlen(param1));
	// union Register data = {0};

	char *param2 = strsep(&tmp, "|");
	printf("%d %s\n", __LINE__, param2);
	// data.val = strtoul(param2, NULL, 16);
	// memcpy(tmp + strlen(param1), data.bytes, 8);

	free(tmp);
	return 0;
}
