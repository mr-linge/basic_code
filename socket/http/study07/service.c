#include "service.h"

void add_http_header(char *header, char *content_type, unsigned long body_length)
{
    strcat(header, "HTTP/1.1 200 OK\r\n");
    strcat(header, "Server: Apache Server V1.0\r\n");
    strcat(header, "Accept-Ranges: bytes\r\n");
    strcat(header, "Connection: close\r\n");
    if (body_length > 0)
    {
        sprintf(header, "%sContent-Length: %lu\r\n", header, body_length);
    }
    if (content_type != NULL)
    {
        sprintf(header, "%sContent-Type: %s\r\n", header, content_type);
    }

    strcat(header, "\r\n");
}

struct HTTP_DATA *get_data(struct HTTP_DATA *request)
{
    printf("%s", request->header);
    puts("");
    printf("%s:%d go here.\n", __FILE__, __LINE__);
    char *http_body = "{\"code\":200,\"msg\":\"This is GET return data\"}";

    struct HTTP_DATA *response = (struct HTTP_DATA *)malloc(sizeof(struct HTTP_DATA));
    memset(response, 0, sizeof(struct HTTP_DATA));
    response->header = (char *)malloc(BUFSIZ);
    response->body = (char *)malloc(strlen(http_body));
    response->body_length = strlen(http_body);

    memcpy(response->body, http_body, strlen(http_body));
    add_http_header(response->header, "application/json", strlen(http_body));
    printf("%s:%d go here.\n", __FILE__, __LINE__);

    return response;
}

struct HTTP_DATA *post_data(struct HTTP_DATA *request)
{
    printf("%s", request->header);
    printf("%s", request->body);
    puts("");
    printf("%s:%d go here.\n", __FILE__, __LINE__);
    char *http_body = "{\"code\":200,\"msg\":\"This is POST return data\"}";
    struct HTTP_DATA *response = (struct HTTP_DATA *)malloc(sizeof(struct HTTP_DATA));
    memset(response, 0, sizeof(struct HTTP_DATA));
    response->header = (char *)malloc(BUFSIZ);
    response->body = (char *)malloc(strlen(http_body));
    response->body_length = strlen(http_body);

    memcpy(response->body, http_body, strlen(http_body));
    add_http_header(response->header, "application/json", strlen(http_body));
    printf("%s:%d go here.\n", __FILE__, __LINE__);

    return response;
}

struct HTTP_DATA *download(struct HTTP_DATA *request)
{
    printf("%s", request->header);

    char http_type[0x10] = {0}, path[0x100] = {0};
    int retVal = sscanf(request->header, "%s %s", http_type, path);
    printf("retVal:%d\n", retVal);
    if (retVal < 0)
    {
        printf("%s:%d match fail\n", __FILE__, __LINE__);
    }

    struct HTTP_DATA *response = (struct HTTP_DATA *)malloc(sizeof(struct HTTP_DATA));
    memset(response, 0, sizeof(struct HTTP_DATA));
    response->header = (char *)malloc(BUFSIZ);
    response->body = (char *)malloc(0x100);
    memset(response->body, 0, 0x100);
    response->body_length = 0x100;

    strcat(response->body, ".");
    strcat(response->body, path);

    struct stat buf;
    int status = stat(response->body, &buf);
    if (status == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }
    // printf("%s file size = %lld\n", response->body, buf.st_size);
    add_http_header(response->header, "application/octet-stream", buf.st_size);
    // printf("%s:%d go here.\n", __FILE__, __LINE__);

    return response;
}

// struct Response *upload(struct Request *request) {
//         printf("%s", request->header);

//     char http_type[0x10] = {0}, path[0x100] = {0};
//     int retVal = sscanf(request->header, "%s %s", http_type, path);
//     printf("retVal:%d\n", retVal);
//     if (retVal < 0)
//     {
//         printf("%s:%d match fail\n", __FILE__, __LINE__);
//     }

//     struct Response *response = (struct Response *)malloc(sizeof(struct Response));
//     memset(response, 0, sizeof(struct Response));
//     response->header = (char *)malloc(BUFSIZ);
//     response->body = (char *)malloc(0x100);
//     memset(response->body, 0, 0x100);
//     response->body_length = 0x100;

//     strcat(response->body, ".");
//     strcat(response->body, path);

//     struct stat buf;
//     int status = stat(response->body, &buf);
//     if (status == -1)
//     {
//         fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
//         exit(-1);
//     }
//     // printf("%s file size = %lld\n", response->body, buf.st_size);
//     add_http_header(response->header, "application/octet-stream", buf.st_size);
//     // printf("%s:%d go here.\n", __FILE__, __LINE__);

//     return response;
// }

// struct HTTP_DATA *get_html(struct HTTP_DATA *request)
// {
//     printf("%s", request->header);

//     char http_type[0x10] = {0}, path[0x100] = {0};
//     int retVal = sscanf(request->header, "%s %s", http_type, path);
//     printf("retVal:%d\n", retVal);
//     if (retVal < 0)
//     {
//         printf("%s:%d match fail\n", __FILE__, __LINE__);
//     }

//     struct Response *response = (struct Response *)malloc(sizeof(struct Response));
//     memset(response, 0, sizeof(struct Response));
//     response->header = (char *)malloc(BUFSIZ);
//     response->body = (char *)malloc(0x100);
//     memset(response->body, 0, 0x100);
//     response->body_length = 0x100;

//     strcat(response->body, ".");
//     strcat(response->body, path);

//     struct stat buf;
//     int status = stat(response->body, &buf);
//     if (status == -1)
//     {
//         fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
//         exit(-1);
//     }
//     // printf("%s file size = %lld\n", response->body, buf.st_size);
//     add_http_header(response->header, "text/html; charset=utf-8", buf.st_size);
//     // printf("%s:%d go here.\n", __FILE__, __LINE__);

//     return response;
// }

struct HTTP_DATA *other(struct HTTP_DATA *request)
{
    printf("%s", request->header);
    if (request->body != NULL)
    {
        printf("%s", request->body);
    }
    puts("");
    printf("%s:%d go here.\n", __FILE__, __LINE__);
    struct HTTP_DATA *response = (struct HTTP_DATA *)malloc(sizeof(struct HTTP_DATA));
    memset(response, 0, sizeof(struct HTTP_DATA));
    response->header = (char *)malloc(BUFSIZ);
    response->body = NULL;
    add_http_header(response->header, NULL, 0);
    printf("%s:%d go here.\n", __FILE__, __LINE__);

    return response;
}