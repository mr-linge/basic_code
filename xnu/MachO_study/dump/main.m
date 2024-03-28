#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#import <mach-o/dyld.h>

void show_all_image_info() {
    unsigned int count = _dyld_image_count();
    const char *image_name;
    unsigned long vmaddr_slide;
    const struct mach_header *image_header = NULL;
    for (unsigned int i = 0 ; i < count; ++i) {
        image_name = _dyld_get_image_name(i);
        vmaddr_slide = _dyld_get_image_vmaddr_slide(i);
        image_header = _dyld_get_image_header(i);
        printf("%3d name:%s vmaddr_slide:0x%lx header:%p\n",i,image_name,vmaddr_slide,image_header);
    }
}


int lookup_module_info(uint32_t *index,char *module_name, int flag)
{
    const char *image_name = NULL;
    for (uint32_t i = 0; i < _dyld_image_count(); i++) {
        image_name = _dyld_get_image_name(i);
        if (flag == 0) {
            if(strstr(image_name, module_name) != NULL) { // 采用模糊匹配,模块名包含module即返回
                *index = i;
				return 0;
            }
        }else {  
            if(strcmp(image_name, module_name) == 0) {   // strcmp 准确匹配
				*index = i;
                return 0;
            }
        } 
    }
    return 1;
}

// 函数指针
typedef int (*CAC_FUNC)(int, int);


int test_func() {
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

	return 0;
}

void dump(void *start_vaddr, void * end_vaddr, const char *path)
{
	int fd = open(path, O_WRONLY | O_CREAT);
	if (fd == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		return;
	}

	printf("start vaddr:%p\n",start_vaddr);
	printf("end   vaddr:%p\n",end_vaddr);
	unsigned long len = (unsigned long)end_vaddr - (unsigned long)start_vaddr;
	printf("len:%lx\n", len);
	unsigned long retval = write(fd, start_vaddr, len);
	if (retval == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		return;
	}
	close(fd);
}

int main(int argc, char *argv[])
{
	printf("****************** pid:%d *******************\n", getpid());
	if(argc != 3) {
		printf("input 1 param!\n");
		return -1;
	}

	for (int i = 0; i < argc; i++)
	{
		printf("argv[%d] = %s\n", i, argv[i]);
	}

	test_func();
	show_all_image_info();

	getchar();

    // char *module_name = "libcaculate.dylib";
    // char *module_name = "main";
	char *module_name = argv[1];
	char *outpath = argv[2];
    // const struct mach_header *image_header = NULL;
	uint32_t index = 0;
    int retval = lookup_module_info(&index, module_name, 0);
	if(retval) {
		printf("%s not find!\n",module_name);
		return -1;
	}
	const struct mach_header *image_header = _dyld_get_image_header(index);
	const struct mach_header *image_header2 = _dyld_get_image_header(index + 1);

	dump((void *)image_header, (void *)image_header2, outpath);
	// dlclose(handle);

	return 0;
}
