#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

/*
   gcc -shared global_var.c -o libglobal.so
 * */

typedef struct {
	const char *module;
	int  (*GetValue)(char *pszVal);
	int   (*PrintfHello)();
} hello_ST_API;


int GetValue(char *pszVal) {
	int retval = -1;

	if (pszVal)
		retval = sprintf(pszVal, "%s", "123456");
	printf("%s, %d, pszVer = %s\n", __FUNCTION__, __LINE__, pszVal);
	return retval;
}

int PrintfHello() {
	int retval = -1;

	printf("%s, %d, hello everyone\n", __FUNCTION__, __LINE__);
	return 0;
}

const hello_ST_API  Hello = {
	.module = "hello",
	GetValue,
	PrintfHello,
};
