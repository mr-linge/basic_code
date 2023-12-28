#include "server.h"
#include "service.h"
#include "http_parse.h"
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>

// 释放 heap 内存
void free_heap(void *vaddr)
{
    if (vaddr != NULL)
    {
        free(vaddr);
    }
}

// // 解析获取到的 http 请求
// struct Request *parse_request(int sock_client)
// {
//     char buff[BUFSIZ] = {0};
//     unsigned long index = 0, len = 0, header_len = 0, content_length = 0;
//     len = recv(sock_client, buff, BUFSIZ, MSG_PEEK); // 接收的第一波数据,包含 http header 和 http body, 需要识别并分离第一波数据
//     // printf("%s:%d len:%lu\n", __FILE__, __LINE__, len);
//     if (len == -1)
//     {
//         fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
//         exit(-1);
//     }
//     char *header_end_position = strstr(buff, "\r\n\r\n"); // http header 和 http body 以 \r\n\r\n 作为分割
//     if (header_end_position == NULL)
//     {
//         printf("%s:%d enter not match\n", __FILE__, __LINE__);
//         return NULL;
//     }
//     header_end_position += strlen("\r\n\r\n"); // header 结束的位置
//     header_len = (unsigned long)(header_end_position - buff);

//     // request 返回的结构体
//     struct Request *request = (struct Request *)malloc(sizeof(struct Request));
//     request->header = (char *)malloc(header_len);
//     request->header_length = header_len;

//     char *header = request->header;
//     len = recv(sock_client, header, header_len, 0);
//     // printf("%s:%d len:%lu\n", __FILE__, __LINE__, len);
//     if (len == -1)
//     {
//         fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
//         exit(-1);
//     }
//     // printf("%s", header);

//     char *content_info = strstr(header, "Content-Length:");
//     if (content_info == NULL)
//     {
//         printf("%s:%d Content-Length not match\n", __FILE__, __LINE__);
//         return request;
//     }
//     content_info += strlen("Content-Length:");
//     content_length = strtoul(content_info, NULL, 10);
//     if (content_length == 0)
//     {
//         printf("%s:%d Content-Length is not a vaild number\n", __FILE__, __LINE__);
//         return request;
//     }

//     /*
//         recv 要保证接受完所有的数据,目前只有根据 header 中 Content-Length 来计算还未接收的数据
//     **/
//     request->body = (char *)malloc(content_length);
//     request->body_length = content_length;
//     char *body = request->body;
//     while (index < content_length)
//     {
//         len = recv(sock_client, body + index, BUFSIZ, 0);
//         if (len == -1)
//         {
//             fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
//             exit(-1);
//         }
//         index += len;
//     }
//     // printf("%s", body);
//     // puts("");

//     return request;
// }

// 给 Client 发送数据
void send_response_data(int sock_client, struct HTTP_DATA *response)
{
    // 发送 http header
    if (send(sock_client, response->header, strlen(response->header), 0) == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }

    // 发送 http body
    if (response->body != NULL)
    {
        printf("%s:%d go here.\n", __FILE__, __LINE__);
        unsigned long index = 0, len = 0, order_len = 0;
        printf("%s:%d response->body_length:%lu\n", __FILE__, __LINE__, response->body_length);
        while (index < response->body_length)
        {
            printf("%s:%d go here.\n", __FILE__, __LINE__);
            order_len = response->body_length - index;
            printf("%s:%d order_len:%lu\n", __FILE__, __LINE__, order_len);
            len = send(sock_client, response->body + index, order_len, 0);
            printf("%s:%d go here.\n", __FILE__, __LINE__);
            if (len == -1)
            {
                fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
                exit(-1);
            }
            index += len;
        }
    }
}

// void send_file(int sock_client, struct HTTP_DATA *response)
// {
//     // 发送 http header
//     if (send(sock_client, response->header, strlen(response->header), 0) == -1)
//     {
//         fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
//         exit(-1);
//     }
//     char buff[BUFSIZ] = {0};

//     int fd = open(response->body, O_RDONLY);
//     if (fd == -1)
//     {
//         fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
//         exit(-1);
//     }

//     unsigned long len = 0;
//     // 循环将文件 fd 中的内容读取到 buff 中
//     while ((len = read(fd, buff, BUFSIZ)) != 0)
//     {
//         if (len == -1) // I/O 错误
//         {
//             fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
//             break;
//         }
//         if (send(sock_client, buff, len, 0) == -1)
//         {
//             fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
//             exit(-1);
//         }
//     }

//     close(fd);
// }

// void save_file(int sock_client, struct HTTP_DATA *response)
// {
//     char buff[BUFSIZ] = {0};
//     unsigned long i, len, write_len = 0;
//     len = recv(sock_client, buff, BUFSIZ, 0); // 接收的第一波数据,包含 http header 和 http body, 需要识别并分离第一波数据
//     if (len == -1)
//     {
//         fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
//         exit(-1);
//     }
//     char *body = strstr(buff, "\r\n\r\n"); // http header 和 http body 以 \r\n\r\n 作为分割
//     if (body == NULL)
//     {
//         printf("%s:%d enter not match\n", __FILE__, __LINE__);
//         return;
//     }
//     body += strlen("\r\n\r\n"); // body 开始的位置

//     char header[BUFSIZ] = {0};
//     memcpy(header, buff, (unsigned long)(body - buff)); // 获取到 http header
//     // printf("%s:%d header length:%lu\n", __FILE__, __LINE__, strlen(header));
//     // for (i = 0; i < strlen(header); i++)
//     // {
//     // 	printf("%c", buff[i]);
//     // }

//     // 准备把接收的数据写到本地文件里
//     int fd = open(response->body, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
//     if (fd == -1)
//     {
//         fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
//         exit(-1);
//     }

//     write_len = write(fd, body, len - strlen(header));
//     if (write_len == -1)
//     {
//         fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
//         exit(-1);
//     }

//     // 获取 header 中的 Content-Length  的数据值
//     char *content_info = strstr(header, "Content-Length:");
//     if (content_info == NULL)
//     {
//         printf("%s:%d Content-Length not match\n", __FILE__, __LINE__);
//         return;
//     }
//     content_info += strlen("Content-Length:");
//     unsigned long content_length = strtoul(content_info, NULL, 10);
//     if (content_length == 0)
//     {
//         printf("%s:%d Content-Length is not a vaild number\n", __FILE__, __LINE__);
//         return;
//     }

//     /*
//         recv 要保证接受完所有的数据,目前只有根据 header 中 Content-Length 来计算还未接收的数据
//     **/
//     unsigned long received_body_length = len - strlen(header);        // 第一次 recv 已经接收到的 content 部分的长度
//     unsigned long remain_len = content_length - received_body_length; // content 还未接收的数据长度
//     // printf("%s:%d remain_len:%lu\n", __FILE__, __LINE__, remain_len);
//     while (remain_len > 0)
//     {
//         len = recv(sock_client, buff, BUFSIZ, 0);
//         // printf("%s:%d len:%lu\n", __FILE__, __LINE__, len);
//         if (len == -1)
//         {
//             fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
//             exit(-1);
//         }
//         write_len = write(fd, buff, len);
//         if (write_len == -1)
//         {
//             fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
//             exit(-1);
//         }

//         remain_len -= len;
//     }
//     close(fd);
// }

// 匹配 uri 对应的方法
void mapping(int sock_client)
{
    struct HTTP_DATA *request = parse_request(sock_client);
    // struct HTTP_HEAD_INFO *header_info = NULL;
    // get_header_info(&header_info, request->header);

    char *uri1 = "GET /get_data";
    char *uri2 = "POST /post_data";
    char *uri3 = "GET /file/";
    char *uri4 = "GET /html/index.html";

    struct HTTP_DATA *response = NULL;
    if (strncmp(request->header, uri1, strlen(uri1)) == 0)
    {
        printf("%s:%d go here.\n", __FILE__, __LINE__);
        response = get_data(request);
        // 响应 Client 请求
        send_response_data(sock_client, response);
    }
    else if (strncmp(request->header, uri2, strlen(uri2)) == 0)
    {
        printf("%s:%d go here.\n", __FILE__, __LINE__);
        response = post_data(request);
        // 响应 Client 请求
        send_response_data(sock_client, response);
    }
    else if (strncmp(request->header, uri3, strlen(uri3)) == 0)
    {
        printf("%s:%d go here.\n", __FILE__, __LINE__);
        response = download(request);
        send_file(sock_client, response);
    }
    else if (strncmp(request->header, uri4, strlen(uri4)) == 0)
    {
        printf("%s:%d go here.\n", __FILE__, __LINE__);
        response = get_html(request);
        send_file(sock_client, response);
    }
    else
    {
        printf("%s:%d go here.\n", __FILE__, __LINE__);
        response = other(request);
        // 响应 Client 请求
        send_response_data(sock_client, response);
    }

    // // 响应 Client 请求
    // send_response_data(sock_client, response);

    free_heap(response->header);
    free_heap(response->body);
    free_heap(response);

    free_heap(request->header);
    free_heap(request->body);
    free_heap(request);
}

int main()
{
    server_run("127.0.0.1", 9000, &mapping);

    return 0;
}
