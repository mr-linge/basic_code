#include "hook_util.h"

pid_t pid;

int onEnter(char *func_name, unsigned long func_vaddr, long long *argv)
{
    printf("onEnter -------> func_name: %s, func_vaddr: %lx <-------\n", func_name, func_vaddr);
    if (strcmp(func_name, "SSL_read") == 0)
    {
        // for (int k = 0; k < 20; k++)
        // {
        //     printf("argv[%d] = 0x%llx\n", k, argv[k]);
        // }
        // argv[1] = 0xaaaaaaaa;
    }

    if (strcmp(func_name, "SSL_write") == 0)
    {
        printf("argv[0] vaddr: 0x%llx\n", argv[0]);
        unsigned long len = 0x100;
        uint8_t *dst = (uint8_t *)malloc(len);
        memset(dst, 0, len);
        // uint8_t str[0x100];
        getdata(pid, argv[0], dst, len);
        printf("argv[0] as string:\n%s\n", dst);
        for (int i = 0; i < len; ++i)
        {
            printf("0x%02X ", *(dst + i));
        }
        puts("");
    }

    return 0;
}

int onLeave(char *func_name, unsigned long func_vaddr, long long *argv)
{
    printf("onLeave -------> func_name: %s, func_vaddr: %lx <-------\n", func_name, func_vaddr);
    if (strcmp(func_name, "SSL_read") == 0)
    {
        printf("return value : 0x%llx\n", *argv);

        unsigned long len = 0x100;
        // uint8_t str[0x100];
        uint8_t *dst = (uint8_t *)malloc(len);
        memset(dst, 0, len);
        getdata(pid, *argv, dst, len);
        printf("return value as string:\n%s\n", dst);
        printf("argv[0]:%s\n", dst);
        for (int i = 0; i < len; ++i)
        {
            printf("0x%02X ", *(dst + i));
        }
        puts("");
    }

    if (strcmp(func_name, "SSL_write") == 0)
    {
        // printf("return value : 0x%llx\n", *argv);
        // *argv = 0xfc21;
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
        return -1;
    }
    signal(SIGINT, sighandler);

    pid = atoi(argv[1]);

    int func_count = 2;
    struct Hooked_func funcs[func_count];

    struct Hooked_func ssl_read;
    memset(&ssl_read, 0, sizeof(struct Hooked_func));
    char *ssl_read_name = "SSL_read";
    memcpy(ssl_read.func_name, ssl_read_name, strlen(ssl_read_name));
    char *ssl_read_module_path = "/system/lib64/libssl.so";
    memcpy(ssl_read.module_path, ssl_read_module_path, strlen(ssl_read_module_path));
    ssl_read.func_vaddr = 0; // 0x5a5726ba00;
    ssl_read.num_params = 1;

    struct Hooked_func ssl_write;
    memset(&ssl_write, 0, sizeof(struct Hooked_func));
    char *ssl_write_name = "SSL_write";
    memcpy(ssl_write.func_name, ssl_write_name, strlen(ssl_write_name));
    char *ssl_write_module_path = "/system/lib64/libssl.so";
    memcpy(ssl_write.module_path, ssl_write_module_path, strlen(ssl_write_module_path));
    ssl_write.func_vaddr = 0; // 0x5a5726bba4;
    ssl_write.num_params = 1;

    funcs[0] = ssl_read;
    funcs[1] = ssl_write;

    hook(pid, funcs, func_count, (unsigned long)&onEnter, (unsigned long)&onLeave);

    return 0;
}