#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h> // read write
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h> // socket
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 9000
int listen_max = 10; // 最大并发数量

const char *file_path = "./test.ipa";

const char *content_type = "application/octet-stream";

void send_http_header(int sock_client, unsigned long http_body_length)
{
	char http_header[BUFSIZ] = {0};

	strcat(http_header, "HTTP/1.1 200 OK\r\n");
	strcat(http_header, "Server: Apache Server V1.0\r\n");
	strcat(http_header, "Accept-Ranges: bytes\r\n");
	strcat(http_header, "Connection: close\r\n");
	if (http_body_length > 0)
	{
		sprintf(http_header, "%sContent-Length: %lu\r\n", http_header, http_body_length);
	}
	sprintf(http_header, "%sContent-Type: %s\r\n", http_header, content_type);

	strcat(http_header, "\r\n"); // \r\n 空行后是 body 数据

	if (send(sock_client, http_header, strlen(http_header), 0) == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
	}
}

void send_http_body(int sock_client, char *body, unsigned long len)
{
	if (send(sock_client, body, len, 0) == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
	}
}

void send_data(int sock_client)
{
	char *http_body = "{\"code\":200,\"msg\":\"file upload success.\"}";
	send_http_header(sock_client, strlen(http_body));
	send_http_body(sock_client, http_body, strlen(http_body));
}

// 解析获取到的 http 请求
void receive_data(int sock_client)
{
	char buff[BUFSIZ] = {0};
	unsigned long i, len, write_len = 0;
	len = recv(sock_client, buff, BUFSIZ, 0); // 接收的第一波数据,包含 http header 和 http body, 需要识别并分离第一波数据
	if (len == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(-1);
	}
	char *body = strstr(buff, "\r\n\r\n"); // http header 和 http body 以 \r\n\r\n 作为分割
	if (body == NULL)
	{
		printf("%s:%d enter not match\n", __FILE__, __LINE__);
		return;
	}
	body += strlen("\r\n\r\n"); // body 开始的位置

	char header[BUFSIZ] = {0};
	memcpy(header, buff, (unsigned long)(body - buff)); // 获取到 http header
	// printf("%s:%d header length:%lu\n", __FILE__, __LINE__, strlen(header));
	for (i = 0; i < strlen(header); i++)
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

	write_len = write(fd, body, len - strlen(header));
	if (write_len == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(-1);
	}

	// 获取 header 中的 Content-Length  的数据值
	char *content_info = strstr(header, "Content-Length:");
	if (content_info == NULL)
	{
		printf("%s:%d Content-Length not match\n", __FILE__, __LINE__);
		return;
	}
	content_info += strlen("Content-Length:");
	unsigned long content_length = strtoul(content_info, NULL, 10);
	if (content_length == 0)
	{
		printf("%s:%d Content-Length is not a vaild number\n", __FILE__, __LINE__);
		return;
	}

	/*
		recv 要保证接受完所有的数据,目前只有根本 header 中 Content-Length 来计算还未接收的数据
	**/
	unsigned long received_body_length = len - strlen(header);		  // 第一次 recv 已经接收到的 content 部分的长度
	unsigned long remain_len = content_length - received_body_length; // content 还未接收的数据长度
	// printf("%s:%d remain_len:%lu\n", __FILE__, __LINE__, remain_len);
	while (remain_len > 0)
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
		memset(buff, '\0', BUFSIZ);

		remain_len -= len;
	}
	close(fd);
}

int main()
{
	// 定义 socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// 定义 sockaddr_in
	struct sockaddr_in skaddr;
	skaddr.sin_family = AF_INET; // ipv4
	skaddr.sin_port = htons(PORT);
	skaddr.sin_addr.s_addr = INADDR_ANY;
	// bind  绑定 socket 和 sockaddr_in
	if (bind(sockfd, (struct sockaddr *)&skaddr, sizeof(skaddr)) == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}

	// listen  开始添加端口
	if (listen(sockfd, listen_max) == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}

	// 客户端信息
	struct sockaddr_in claddr;
	socklen_t length = sizeof(claddr);

	int sock_client = 0;
	while (1)
	{
		sock_client = accept(sockfd, (struct sockaddr *)&claddr, &length);
		if (sock_client < 0)
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			break;
		}

		// 接收传送过来的 http 数据
		receive_data(sock_client);

		// 响应 http 请求,回传 http 数据
		send_data(sock_client);

		close(sock_client);
	}
	close(sockfd);

	return 0;
}
