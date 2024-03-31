#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>

// 函数指针
typedef int (*CAC_FUNC)(int, int);

int main()
{
	printf("****************** pid:%d *******************\n", getpid());

	void *handle;
	char *error;
	CAC_FUNC cac_func = NULL;

	const char *lib_path = "./libcaculate.so";
	// 打开动态链接库
	handle = dlopen(lib_path, RTLD_LAZY);
	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, error);
		return -1;
	}

	// 获取一个函数
	int (*cac_func1)(int, int) = dlsym(handle, "add");
	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, error);
		return -1;
	}
	printf("add: %d\n", cac_func1(2, 7));

	cac_func = (CAC_FUNC)dlsym(handle, "sub");
	printf("%d cac_func: %p\n", __LINE__, cac_func);
	printf("sub: %d\n", cac_func(9, 2));

	cac_func = (CAC_FUNC)dlsym(handle, "mul");
	printf("%d cac_func: %p\n", __LINE__, cac_func);
	printf("mul: %d\n", cac_func(3, 2));

	cac_func = dlsym(handle, "div");
	printf("%d cac_func: %p\n", __LINE__, cac_func);
	printf("div: %d\n", cac_func(8, 2));

	Dl_info *info = (Dl_info *)malloc(sizeof(Dl_info));
	int status = dladdr(cac_func, info);
	printf("status:%d\n",status);
	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, error);
		return -1;
	}
	printf("dli_fname:%s\n",info->dli_fname);
	printf("dli_fbase:%p\n",info->dli_fbase);
	printf("dli_sname:%s\n",info->dli_sname);
	printf("dli_saddr:%p\n",info->dli_saddr);

	status = dladdr(&printf, info);
	printf("status:%d\n",status);
	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, error);
		return -1;
	}
	printf("dli_fname:%s\n",info->dli_fname);
	printf("dli_fbase:%p\n",info->dli_fbase);
	printf("dli_sname:%s\n",info->dli_sname);
	printf("dli_saddr:%p\n",info->dli_saddr);

	// 关闭动态链接库,关闭后该动态库就会从 当前进程中 移除
	dlclose(handle);

	return 0;
}
