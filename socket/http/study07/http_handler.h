#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <errno.h>
#include <unistd.h> // read write
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h> // socket
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct HTTP_DATA
{
    char *header;
    unsigned long header_length;
    char *body;
    unsigned long body_length;
};

unsigned long get_content_length_http_header(char *header);
void add_http_header(char *header, char *content_type, unsigned long content_length);

// 解析获取到的 http 请求
int get_request_data(int sock_client, struct HTTP_DATA **data);

// 给客护端发送数据
int send_response_data(int sock_client, char *buff, unsigned long buff_len);

// 发送文件
int send_file(int sock_client, char *file_path);

// 接收文件
int save_file(int sock_client, char *file_path);
