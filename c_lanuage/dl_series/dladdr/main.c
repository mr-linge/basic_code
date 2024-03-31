#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>

// 函数指针
typedef int (*CAC_FUNC)(int, int);

int func_max(int p1, int p2)
{
	return p1 > p2 ? p1 : p2;
}

int main()
{
	printf("****************** pid:%d *******************\n", getpid());

	void *handle;
	char *error;
	CAC_FUNC cac_func = NULL;
	int status = 0, p1 = 20, p2 = 10;

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
	printf("%d add 			vaddr:%p\n", __LINE__, cac_func1);
	printf("%d add(%d,%d) = %d\n", __LINE__, p1, p2, cac_func1(p1, p2));

	cac_func = dlsym(handle, "div");
	printf("%d div 			vaddr:%p\n", __LINE__, cac_func);
	printf("%d div(%d,%d) = %d\n", __LINE__, p1, p2, cac_func(p1, p2));

	printf("%d func_max 	vaddr:%p\n", __LINE__, &func_max);
	printf("%d func_max(%d,%d) = %d\n", __LINE__, p1, p2, func_max(p1, p2));

	Dl_info *info = (Dl_info *)malloc(sizeof(Dl_info));
	status = dladdr(cac_func, info);
	printf("status:%d\n", status);
	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, error);
		return -1;
	}
	printf("dli_fname:%s\n", info->dli_fname);
	printf("dli_fbase:%p\n", info->dli_fbase);
	printf("dli_sname:%s\n", info->dli_sname);
	printf("dli_saddr:%p\n", info->dli_saddr);

	status = dladdr(&printf, info);
	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, error);
		return -1;
	}
	printf("dli_fname:%s\n", info->dli_fname);
	printf("dli_fbase:%p\n", info->dli_fbase);
	printf("dli_sname:%s\n", info->dli_sname);
	printf("dli_saddr:%p\n", info->dli_saddr);

	status = dladdr(&func_max, info);
	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, error);
		return -1;
	}
	printf("dli_fname:%s\n", info->dli_fname);
	printf("dli_fbase:%p\n", info->dli_fbase);
	printf("dli_sname:%s\n", info->dli_sname);
	printf("dli_saddr:%p\n", info->dli_saddr);

	// 关闭动态链接库,关闭后该动态库就会从 当前进程中 移除
	dlclose(handle);

	return 0;
}
