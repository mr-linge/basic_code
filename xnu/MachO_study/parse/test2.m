#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#import <mach-o/dyld.h>
#include "parse.h"

int lookup_module_info(const struct mach_header **image_header, char *module_name, int flag)
{
    const char *image_name = NULL;
    for (uint32_t i = 0; i < _dyld_image_count(); i++) {
        image_name = _dyld_get_image_name(i);
        if (flag == 0) {
            if(strstr(image_name, module_name) != NULL) { // 采用模糊匹配,模块名包含module即返回
                *image_header = _dyld_get_image_header(i);
                return 0;
            }
        }else {  
            if(strcmp(image_name, module_name) == 0) {   // strcmp 准确匹配
                *image_header = _dyld_get_image_header(i);
                return 0;
            }
        } 
    }
    return -1;
}

// 函数指针
typedef int (*CAC_FUNC)(int, int);

// 调用可执行文件里面的函数
int c_test_func(int num)
{
    return num + 10;
}

int main(int argc, char *argv[])
{
	printf("****************** pid:%d *******************\n", getpid());
	if(argc != 2) {
		printf("input 1 param!\n");
		return -1;
	}

	void *handle;
	char *error;
	int retval = 0, p1 = 20, p2 = 10;
	CAC_FUNC cac_func = NULL;

	const char *lib_path = "./libcaculate.dylib";
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
	printf("sub: %d\n", cac_func(9, 2));

	cac_func = (CAC_FUNC)dlsym(handle, "mul");
	printf("mul: %d\n", cac_func(3, 2));

	cac_func = dlsym(handle, "div");
	printf("div: %d\n", cac_func(8, 2));

    retval = c_test_func(p1);
    printf("%s:%d c_test_func(%d) = %d\n", __FILE__, __LINE__, p1, retval);

	getchar();

    // char *module_name = "libcaculate.dylib";
    // char *module_name = "main";
	char *module_name = argv[1];
    const struct mach_header *image_header = NULL;
    retval = lookup_module_info(&image_header, module_name, 0);
    if(retval < 0) {
        printf("%s not find!\n", module_name);
        return -1;
    }
    parse((void *)image_header);

	dlclose(handle);

	return 0;
}
