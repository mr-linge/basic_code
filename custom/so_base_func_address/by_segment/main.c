#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

size_t get_module_base(int pid, char *moduleName)
{
    size_t moduleAddr;
    char* buf;
    char* end;
    char mapfile[0x18];
    sprintf(mapfile, "/proc/%d/maps", pid);
    FILE* fd = fopen(mapfile, "r");
    if(!fd)
    {
        printf("open maps error!");
        exit(1);
    }
    //search the libc-.....
    buf = (char*) malloc(0x100);
    do{
        fgets(buf, 0x100, fd);
    } while(!strstr(buf, moduleName));
    end = strchr(buf, '-');
    moduleAddr = strtol(buf, &end, 16);
    printf("The process %d's module base is: 0x%lx\n", pid, moduleAddr);
    fclose(fd);

    return moduleAddr;
}

size_t get_func_offset(int pid, char *funcName)
{
    //get function offset from self process, the shared libc.so
    size_t funcAddr   = (size_t)dlsym(0, funcName);
    //funcAddr &= 0x0000ffffffffffff;
    size_t moduleAddr = get_module_base(getpid(),"libc-");
    size_t offset     = funcAddr - moduleAddr;
    printf("function %s funcAddr: 0x%lx,moduleAddr: 0x%lx,offset: 0x%lx\n",funcName,funcAddr,moduleAddr,offset);
    
    return offset;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
        exit(1);
    }

    pid_t target_pid = atoi(argv[1]);
    char * funcName = argv[2];
    printf("funcName : %s\n", funcName);

    int fd;
    void * start;
    struct stat sb;
    fd = open("/etc/profile", O_RDONLY); /* 打开/etc/passwd */
    fstat(fd, &sb); /* 取得文件大小 */
    start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(start == MAP_FAILED) /* 判断是否映射成功 */
    {
	    perror("mmap init fail");
	    exit(-1);
    }

    char *moduleName = "libc-";
    size_t base = get_module_base(target_pid,moduleName);
    printf("base = 0x%lx\n", base);

    size_t offset = get_func_offset(target_pid,funcName);
    offset &= 0xffffff;
    printf("offset = 0x%lx\n", offset);

    size_t vaddr = base + offset;
    printf("vaddr  = 0x%lx\n", vaddr);

    return 0;
}

