#include "http_parse.h"

// 解析获取到的 http 请求
struct HTTP_DATA *parse_request(int sock_client)
{
    char buff[BUFSIZ] = {0};
    unsigned long index = 0, len = 0, header_len = 0, content_length = 0;
    len = recv(sock_client, buff, BUFSIZ, MSG_PEEK); // 接收的第一波数据,包含 http header 和 http body, 需要识别并分离第一波数据
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
        return NULL;
    }
    header_end_position += strlen("\r\n\r\n"); // header 结束的位置
    header_len = (unsigned long)(header_end_position - buff);

    struct HTTP_DATA *request = (struct HTTP_DATA *)malloc(sizeof(struct HTTP_DATA));
    request->header = (char *)malloc(header_len);
    request->header_length = header_len;

    char *header = request->header;
    len = recv(sock_client, header, header_len, 0);
    // printf("%s:%d len:%lu\n", __FILE__, __LINE__, len);
    if (len == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }
    // printf("%s", header);

    // 获取 header 中部分有用的信息
    struct HTTP_HEAD_INFO *header_info = NULL;
    get_header_info(&header_info, header);
    content_length = header_info->content_length;
    
    request->body = (char *)malloc(content_length);
    request->body_length = content_length;
    char *body = request->body;
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

    return request;
}

int get_header_info(struct HTTP_HEAD_INFO **header_info, char *header)
{
    *header_info = (struct HTTP_HEAD_INFO *)malloc(sizeof(struct HTTP_HEAD_INFO));
    memset(*header_info, 0, sizeof(struct HTTP_HEAD_INFO));

    int retVal = sscanf(header, "%s %s", (*header_info)->http_type, (*header_info)->router);
    // printf("retVal:%d\n", retVal);
    if (retVal < 0)
    {
        printf("%s:%d match fail\n", __FILE__, __LINE__);
        return -1;
    }

    char *content_type = strstr(header, "Content-Type: ");
    if (content_type != NULL)
    {
        content_type += strlen("Content-Type: ");

        char *months[] = {"application/json", "multipart/form-data", NULL};
        unsigned long index = 0;
        char *item = NULL;
        while ((item = months[index]) != NULL)
        {
            //            printf("%s\n", item);
            if (strncmp(content_type, item, strlen(item)) == 0)
            {
                memcpy((*header_info)->content_type, item, strlen(item));
            }
            index++;
        }
    }
    else
    {
        printf("%s:%d Content-Length not match\n", __FILE__, __LINE__);
    }

    char *content_info = strstr(header, "Content-Length:");
    if (content_info != NULL)
    {
        content_info += strlen("Content-Length:");
        unsigned long content_length = strtoul(content_info, NULL, 10);
        if (content_length == 0)
        {
            printf("%s:%d Content-Length is not a vaild number\n", __FILE__, __LINE__);
        }
        (*header_info)->content_length = content_length;
    }
    else
    {
        printf("%s:%d Content-Length not match\n", __FILE__, __LINE__);
    }

    return 0;
}

char *get_value_from_http_header(char *http_header, char *key)
{
    //     char *content_info = strstr(header, "Content-Length:");
    // if (content_info == NULL)
    // {
    //     printf("%s:%d Content-Length not match\n", __FILE__, __LINE__);
    //     return request;
    // }
    // content_info += strlen("Content-Length:");
    // content_length = strtoul(content_info, NULL, 10);
    // if (content_length == 0)
    // {
    //     printf("%s:%d Content-Length is not a vaild number\n", __FILE__, __LINE__);
    //     return NULL;
    // }
    return NULL;
}
