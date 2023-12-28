#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h> // read write
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h> // socket
#include <sys/shm.h>
#include <sys/stat.h>
#include "http_handler.h"

// GET 请求
int get_data(int sock_client);
// POST 请求
int post_data(int sock_client);

// 下载文件
int download(int sock_client);
// 上传文件
int upload(int sock_client);

// 返回 html
int get_html(int sock_client);

int other(int sock_client);
