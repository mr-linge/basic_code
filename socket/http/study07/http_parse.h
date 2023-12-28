#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <errno.h>
#include <unistd.h> // read write
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h> // socket

#include "common_header.h"

// 解析获取到的 http 请求
struct HTTP_DATA *parse_request(int sock_client);

int get_header_info(struct HTTP_HEAD_INFO **header_info, char *header);

char *get_value_from_http_header(char *http_header, char *key);
char *set_key_value_to_http_header(char *http_header, char *key, char *value);
