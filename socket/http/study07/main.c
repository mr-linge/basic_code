#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "server.h"
#include "service.h"

// 释放 heap 内存
void free_heap(void *vaddr)
{
    if (vaddr != NULL)
    {
        free(vaddr);
    }
}

// 匹配 uri 对应的方法
void mapping(int sock_client)
{
    char buff[0x100] = {0};
    unsigned len = recv(sock_client, buff, 0x100, MSG_PEEK);
    if (len == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }

    char *uri1 = "GET /get_data";
    char *uri2 = "POST /post_data";
    char *uri3 = "GET /download";
    char *uri4 = "GET /html/index.html";
    char *uri5 = "POST /upload";

    if (strncmp(buff, uri1, strlen(uri1)) == 0)
    {
        printf("%s:%d go here.\n", __FILE__, __LINE__);
        get_data(sock_client);
    }
    else if (strncmp(buff, uri2, strlen(uri2)) == 0)
    {
        printf("%s:%d go here.\n", __FILE__, __LINE__);
        post_data(sock_client);
    }
    else if (strncmp(buff, uri3, strlen(uri3)) == 0)
    {
        printf("%s:%d go here.\n", __FILE__, __LINE__);
        download(sock_client);
    }
    else if (strncmp(buff, uri4, strlen(uri4)) == 0)
    {
        printf("%s:%d go here.\n", __FILE__, __LINE__);
        get_html(sock_client);
    }
    else if (strncmp(buff, uri5, strlen(uri5)) == 0)
    {
        printf("%s:%d go here.\n", __FILE__, __LINE__);
        upload(sock_client);
    }
    else
    {
        printf("%s:%d go here.\n", __FILE__, __LINE__);
        other(sock_client);
    }

    close(sock_client);
}

int main()
{
    server_run("127.0.0.1", 9000, &mapping);

    return 0;
}
