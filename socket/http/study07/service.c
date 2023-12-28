#include "service.h"

int get_data(int sock_client)
{
    struct HTTP_DATA *request = NULL;
    get_request_data(sock_client, &request);

    printf("%s", request->header);

    char *http_body = "{\"code\":200,\"msg\":\"This is GET return data\"}";
    char *http_header = (char *)malloc(BUFSIZ);
    add_http_header(http_header, "application/json", strlen(http_body));

    send_response_data(sock_client, http_header, strlen(http_header));
    send_response_data(sock_client, http_body, strlen(http_body));

    return 0;
}

int post_data(int sock_client)
{
    struct HTTP_DATA *request = NULL;
    get_request_data(sock_client, &request);

    printf("%s", request->header);
    printf("%s", request->body);

    char *http_body = "{\"code\":200,\"msg\":\"This is POST return data\"}";
    char *http_header = (char *)malloc(BUFSIZ);
    add_http_header(http_header, "application/json", strlen(http_body));

    send_response_data(sock_client, http_header, strlen(http_header));
    send_response_data(sock_client, http_body, strlen(http_body));

    return 0;
}

int download(int sock_client)
{
    struct HTTP_DATA *request = NULL;
    get_request_data(sock_client, &request);

    printf("%s", request->header);

    char *file_path = "./file/test.ipa";

    struct stat buf;
    int status = stat(file_path, &buf);
    if (status == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }

    char *file_name = "test.ipa";
    char *http_header = (char *)malloc(BUFSIZ);
    strcat(http_header, "HTTP/1.1 200 OK\r\n");
    strcat(http_header, "Server: Apache Server V1.0\r\n");
    strcat(http_header, "Accept-Ranges: bytes\r\n");
    strcat(http_header, "Connection: close\r\n");
    sprintf(http_header, "%sContent-Length: %lld\r\n", http_header, buf.st_size);
    sprintf(http_header, "%sContent-Type: %s\r\n", http_header, "application/octet-stream");
    sprintf(http_header, "%sContent-Disposition: attachment; filename=%s\r\n", http_header, file_name);
    strcat(http_header, "\r\n");

    send_response_data(sock_client, http_header, strlen(http_header));
    send_file(sock_client, file_path);

    return 0;
}

int upload(int sock_client)
{
    char *file_path = "./upload/test.ipa";
    save_file(sock_client, file_path);

    char *http_body = "{\"code\":200,\"msg\":\"file upload success\"}";
    char *http_header = (char *)malloc(BUFSIZ);

    add_http_header(http_header, "application/json", strlen(http_body));

    send_response_data(sock_client, http_header, strlen(http_header));

    send_response_data(sock_client, http_body, strlen(http_body));

    return 0;
}

int get_html(int sock_client)
{
    struct HTTP_DATA *request = NULL;
    get_request_data(sock_client, &request);

    printf("%s", request->header);

    char *file_path = "./html/index.html";

    struct stat buf;
    int status = stat(file_path, &buf);
    if (status == -1)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }

    char *http_header = (char *)malloc(BUFSIZ);
    add_http_header(http_header, "text/html; charset=utf-8", buf.st_size);
    send_response_data(sock_client, http_header, strlen(http_header));
    send_file(sock_client, file_path);

    return 0;
}

int other(int sock_client)
{
    struct HTTP_DATA *request = NULL;
    get_request_data(sock_client, &request);

    printf("%s", request->header);

    char *http_body = "{\"code\":400,\"msg\":\"Not find web api\"}";
    char *http_header = (char *)malloc(BUFSIZ);
    add_http_header(http_header, "application/json", strlen(http_body));

    send_response_data(sock_client, http_header, strlen(http_header));
    send_response_data(sock_client, http_body, strlen(http_body));

    return 0;
}