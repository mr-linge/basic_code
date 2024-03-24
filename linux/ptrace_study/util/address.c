#include "address.h"

unsigned long get_module_vaddr(pid_t pid, char *module)
{
    char mapfile[0x20] = {0};
    sprintf(mapfile, "/proc/%d/maps", pid);
    FILE *fd = fopen(mapfile, "r");
    if (!fd)
    {
        fprintf(stderr, "%s:%d open error: %s\n", __FILE__, __LINE__, strerror(errno));
        return 0;
    }
    char buf[0x400] = {0};
    // 按行依次读取文件所有内容
    while (fgets(buf, 0x400, fd) != NULL)
    {
        if (strstr(buf, module) != NULL) // 这里采用模匹配,模块名包含module即返回,如果需要准确匹配可用 strcmp
        {
            break;
        }
    }

    unsigned long module_vaddr = strtol(buf, NULL, 16);
    fclose(fd);

    return module_vaddr;
}

unsigned long get_symbol_offset(char *module, char *symbol, int type)
{
    void *handle = NULL;
    void *vaddr;
    char *error;
    unsigned long offset;
    // type = 0 则是系统库而且已经加载到进程了
    if (type != 0)
    {
        handle = dlopen(module, RTLD_LAZY);
        if ((error = dlerror()) != NULL)
        {
            fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, error);
            exit(1);
        }
    }
    // printf("handle = %p\n", handle);
    vaddr = dlsym(handle, symbol);
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, error);
        exit(1);
    }
    // printf("vaddr = 0x%lx\n", (unsigned long)vaddr);

    unsigned long module_vaddr = get_module_vaddr(getpid(), module);
    // printf("module_vaddr = 0x%lx\n", module_vaddr);

    offset = (unsigned long)vaddr - module_vaddr;
    if (type != 0)
    {
        dlclose(handle);
    }
    return offset;
}
