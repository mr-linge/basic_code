#include "http_handler.h"

unsigned long get_content_length_http_header(char *header)
{
    unsigned long content_length = 0;
    char *content_info = strstr(header, "Content-Length:");
    if (content_info == NULL)
    {
        printf("%s:%d Content-Length not match\n", __FILE__, __LINE__);
        return 0;
    }
    content_info += strlen("Content-Length:");
    content_length = strtoul(content_info, NULL, 10);
    if (content_length == 0)
    {
        printf("%s:%d Content-Length is not a vaild number\n", __FILE__, __LINE__);
    }

    return content_length;
}

void add_http_header(char *header, char *content_type, unsigned long content_length)
{
    strcat(header, "HTTP/1.1 200 OK\r\n");
    strcat(header, "Server: Apache Server V1.0\r\n");
    strcat(header, "Accept-Ranges: bytes\r\n");
    strcat(header, "Connection: close\r\n");
    if (content_length > 0)
    {
        sprintf(header, "%sContent-Length: %lu\r\n", header, content_length);
    }
    if (content_type != NULL)
    {
        sprintf(header, "%sContent-Type: %s\r\n", header, content_type);
    }

    strcat(header, "\r\n");
}

int get_request_data(int sock_client, struct HTTP_DATA **data)
{
    char buff[BUFSIZ] = {0};
    unsigned long index = 0, len = 0, header_len = 0, content_length = 0;
    len = recv(sock_client, buff, BUFSIZ, MSG_PEEK);
    // printf("%s:%d len:%lu\n", __FILE__, __LINE__, len);
    if (len == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }
    char *header_end_position = strstr(buff, "\r\n\r\n"); // http header 和 http body 以 \r\n\r\n 作为分割
    if (header_end_position == NULL)
    {
        printf("%s:%d enter not match\n", __FILE__, __LINE__);
        return -1;
    }
    header_end_position += strlen("\r\n\r\n"); // header 结束的位置
    header_len = (unsigned long)(header_end_position - buff);

    *data = (struct HTTP_DATA *)malloc(sizeof(struct HTTP_DATA));
    (*data)->header = (char *)malloc(header_len);
    (*data)->header_length = header_len;

    char *header = (*data)->header;
    len = recv(sock_client, header, header_len, 0);
    // printf("%s:%d len:%lu\n", __FILE__, __LINE__, len);
    if (len == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }
    // printf("%s", header);

    // 获取 header Content-Length
    content_length = get_content_length_http_header(header);
    if (content_length <= 0)
    {
        return 0;
    }

    (*data)->body = (char *)malloc(content_length);
    (*data)->body_length = content_length;
    char *body = (*data)->body;
    while (index < content_length)
    {
        len = recv(sock_client, body + index, BUFSIZ, 0);
        if (len == -1)
        {
            fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
            exit(-1);
        }
        index += len;
    }
    // printf("%s", body);
    // puts("");

    return 0;
}

int send_response_data(int sock_client, char *buff, unsigned long buff_len)
{
    unsigned long index = 0, len = 0, order_len = 0;
    while (index < buff_len)
    {
        order_len = buff_len - index;
        // printf("%s:%d order_len:%lu\n", __FILE__, __LINE__, order_len);
        len = send(sock_client, buff + index, order_len, 0);
        // printf("%s:%d go here.\n", __FILE__, __LINE__);
        if (len == -1)
        {
            fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
            return -1;
        }
        index += len;
    }
    return 0;
}

int send_file(int sock_client, char *file_path)
{
    char buff[BUFSIZ] = {0};
    int fd = open(file_path, O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }
    unsigned long len = 0;
    // 循环将文件 fd 中的内容读取到 buff 中
    while ((len = read(fd, buff, BUFSIZ)) != 0)
    {
        if (len == -1) // I/O 错误
        {
            fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
            break;
        }
        if (send(sock_client, buff, len, 0) == -1)
        {
            fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
            exit(-1);
        }
    }
    close(fd);

    return 0;
}

int save_file(int sock_client, char *file_path)
{
    char buff[BUFSIZ] = {0};
    unsigned long index = 0, len = 0, header_len = 0, content_length = 0, write_len = 0;
    len = recv(sock_client, buff, BUFSIZ, MSG_PEEK);
    // printf("%s:%d len:%lu\n", __FILE__, __LINE__, len);
    if (len == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }
    char *header_end_position = strstr(buff, "\r\n\r\n"); // http header 和 http body 以 \r\n\r\n 作为分割
    if (header_end_position == NULL)
    {
        printf("%s:%d enter not match\n", __FILE__, __LINE__);
        return -1;
    }
    header_end_position += strlen("\r\n\r\n"); // header 结束的位置
    header_len = (unsigned long)(header_end_position - buff);

    len = recv(sock_client, buff, header_len, 0);
    // printf("%s:%d len:%lu\n", __FILE__, __LINE__, len);
    if (len == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }
    for (int i = 0; i < len; i++)
    {
        printf("%c", buff[i]);
    }

    // 准备把接收的数据写到本地文件里
    int fd = open(file_path, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }

    // 获取 header Content-Length
    content_length = get_content_length_http_header(buff);

    while (index < content_length)
    {
        len = recv(sock_client, buff, BUFSIZ, 0);
        // printf("%s:%d len:%lu\n", __FILE__, __LINE__, len);
        if (len == -1)
        {
            fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
            exit(-1);
        }
        write_len = write(fd, buff, len);
        if (write_len == -1)
        {
            fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
            exit(-1);
        }
        index += len;
    }
    close(fd);

    // printf("%s", body);
    // puts("");

    return 0;
}