#include <stdio.h>
#include <stdlib.h>
#import <mach-o/dyld.h>

#define PAGEZERO 0x100000000

/*
遍历所有加载到进程的模块(动态库和可执行文件在进程中都称模块)

可执行文件
模块基址
module base vmaddr = vmaddr_slide + PAGEZERO
模块中的符号地址
symbol vmaddr = module base vmaddr + offset 

动态库
模块基址
module base vmaddr = vmaddr_slide
模块中的符号地址
symbol vmaddr = module base vmaddr + offset 

所有模块基址 都等于 image header在内存中的起始位置
module base vmaddr = image_header
**/
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

int func_max(int p1, int p2) {
    return p1 > p2 ? p1 : p2;
}

int main(int argc, const char * argv[]) {
    unsigned int index = 1;
    if(argc == 2) {
        index = (unsigned int)atoi(argv[1]);
    }

    show_all_image_info();

    puts("===============================================");
    const char *image_name = _dyld_get_image_name(index);
    unsigned long vmaddr_slide = _dyld_get_image_vmaddr_slide(index);
    printf("%d %s vmaddr_slide:0x%lx\n",__LINE__,image_name,vmaddr_slide);

    printf("%d func_max         vmaddr:%p\n", __LINE__, &func_max);
    unsigned long func_max_offset = (unsigned long)&func_max - vmaddr_slide - PAGEZERO;
    printf("%d func_max         offset:0x%lx\n", __LINE__, func_max_offset);
    printf("%d main             vmaddr:%p\n", __LINE__, &main);
    unsigned long main_offset = (unsigned long)&main - vmaddr_slide - PAGEZERO;
    printf("%d main             offset:0x%lx\n", __LINE__, main_offset);

    int p1 = 20, p2 = 10;
    int retval = func_max(p1, p2);
    printf("%d func_max(%d,%d) = %d\n", __LINE__, p1, p2, retval);

    return 0;
}
