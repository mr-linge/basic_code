#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>


//动态链接库路径
#define LIB_CACULATE_PATH "./libcaculate.so"

//函数指针
typedef int (*CAC_FUNC)(int, int);

int main() {
	printf("****************** pid:%d *******************\n", getpid());

	void *handle;
	char *error;
	CAC_FUNC cac_func = NULL;

	//打开动态链接库
	handle = dlopen(LIB_CACULATE_PATH, RTLD_LAZY);
	printf("handle = %p\n",handle);
	if (!handle) {
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	//清除之前存在的错误
	dlerror();

	//获取一个函数
	int (*cac_func1)(int,int) = dlsym(handle, "add");
	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		exit(EXIT_FAILURE);
	}
	printf("add: %d\n", cac_func1(2,7));

	// while(1);

	//关闭动态链接库,关闭后该动态库就会从 当前进程中 移除
	dlclose(handle);
	//while(1);

	return 0;
}
