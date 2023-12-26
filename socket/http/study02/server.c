#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h> // read write
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h> // socket
#include <sys/shm.h>

#define PORT 9000
int listen_max = 20; // 最大并发数量

const char *content_type = "application/json";

void send_http_header(int sock_client, unsigned long body_length)
{
	char header[BUFSIZ] = {0};

	strcat(header, "HTTP/1.1 200 OK\r\n");
	strcat(header, "Server: Apache Server V1.0\r\n");
	strcat(header, "Accept-Ranges: bytes\r\n");
	strcat(header, "Connection: close\r\n");
	if (body_length > 0)
	{
		sprintf(header, "%sContent-Length: %lu\r\n", header, body_length);
	}
	sprintf(header, "%sContent-Type: %s\r\n", header, content_type);

	strcat(header, "\r\n"); // http header 和 body 以空行为分隔 \r\n 换行后是 body
	if (send(sock_client, header, strlen(header), 0) == -1)
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
	char *http_body = "{\"code\":200,\"msg\":\"success\",\"data\":\"you will be success!\"}";
	send_http_header(sock_client, strlen(http_body));
	send_http_body(sock_client, http_body, strlen(http_body));
}

// 解析获取到的 http 请求
void receive_data(int sock_client)
{
	char buff[BUFSIZ] = {0};
	unsigned long i, len = 0;
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
	unsigned long received_body_length = len - strlen(header); // 第一次 recv 已经接收到的 content 部分的长度
	for (i = 0; i < received_body_length; i++)
	{
		printf("%c", body[i]);
	}
	puts("");
	unsigned long remain_len = content_length - received_body_length; // content 还未接收的数据长度
	// printf("%s:%d remain_len:%lu\n", __FILE__, __LINE__, remain_len);
	while (remain_len > 0)
	{
		len = recv(sock_client, buff, BUFSIZ, 0);
		if (len == -1)
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(-1);
		}
		// printf("%s:%d len:%lu\n", __FILE__, __LINE__, len);
		for (i = 0; i < len; i++)
		{
			printf("%c", buff[i]);
		}
		printf("\n");

		remain_len -= len;
		// printf("%s:%d remain_len:%lu\n", __FILE__, __LINE__, remain_len);
	}
	// printf("%s:%d remain_len:%lu\n", __FILE__, __LINE__, remain_len);
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

	int sock_client;
	unsigned long i, len;
	while (1)
	{
		sock_client = accept(sockfd, (struct sockaddr *)&claddr, &length);
		if (sock_client < 0)
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(-1);
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
