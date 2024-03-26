#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <spawn.h>

#import <mach-o/dyld.h>

#define PAGEZERO 0x100000000

/*
Header: #include <spawn.h>
Function:
int posix_spawn(pid_t *pid, 
                const char *path,
                const posix_spawn_file_actions_t *file_actions,
                const posix_spawnattr_t *attrp,
                char *const argv[], 
                char *const envp[]);

Description: 根据指定的进程映像创建子进程

参数:
pid         :     子进程pid(pid 参数指向一个缓冲区,该缓冲区用于返回新的子进程的进程ID)
path        :     可执行文件的路径path(也可以调用某些系统命令,只不过要指定其完整路径)
file_actions:     参数指向生成文件操作对象,该对象指定要在子对象之间执行的与文件相关的操作
attrp       :     指向一个属性对象,该对象指定创建的子进程的各种属性
argv        :     指定在子进程中执行的程序的参数列表
envp        :     指定在子进程中执行的程序的环境
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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <path>\n", argv[0]);
        return 1;
    }
    printf("file:%s, line:%d, pid:%d\n", __FILE__, __LINE__, getpid());
    getchar();
    printf("posix_spawn vaddr:%p\n",posix_spawn);
    show_all_image_info();
    pid_t pid;
    char *path = argv[1];
    char *const argv_new[] = {"param1", "param2", NULL};
    char *const envp_new[] = {"AA=11", "BB=22", NULL};
    posix_spawn(&pid, path, NULL, NULL, argv_new, envp_new);
    int stat;
    waitpid(pid,&stat,0);
    printf("file:%s, line:%d, pid:%d, stat:%d\n", __FILE__, __LINE__, getpid(),stat);

    return 0;
}
