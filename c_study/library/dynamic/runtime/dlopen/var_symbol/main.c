#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

/*
   gcc main.c -o main -ldl
 * */

typedef struct {
	const char *module;
	int  (*GetValue)(char *pszVal);
	int   (*PrintfHello)();
} hello_ST_API;

/*
   dlsym找到全局结构体hello后，可以直接用这个全局结构体指针来使用库里面的函数了，因为我们有时候提供的库不仅仅是一个两个函数的，一般的一个库都会存在多个函数，用这种方式就可以直接使用了。不然找函数名称的话要写多个dlsym
 * */
int main(int argc, char **argv) {
	hello_ST_API *hello;
	int i = 0;
	void *handle;
	char psValue[20] = {0};

	handle = dlopen("./libglobal.so", RTLD_LAZY);
	if (!handle) {
		printf("%s,%d, NULL == handle\n", __FUNCTION__, __LINE__);
		return -1;
	}
	dlerror();

	hello = dlsym(handle, "Hello");
	if (!hello) {
		printf("%s,%d, NULL == handle\n", __FUNCTION__, __LINE__);
		return -1;
	}

	if (hello && hello->PrintfHello)
		i = hello->PrintfHello();
	printf("%s, %d, i = %d\n", __FUNCTION__, __LINE__, i);
	if (hello && hello->GetValue)
		i = hello->GetValue(psValue);
	printf("%s, %d, i = %d\n", __FUNCTION__, __LINE__, i);

	if (hello && hello->module) {
		printf("%s, %d, module = %s\n", __FUNCTION__, __LINE__, hello->module);
	}

	dlclose(handle);
	return 0;
}
