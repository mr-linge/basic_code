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
		return;
	}
	header_end_position += strlen("\r\n\r\n"); // header 结束的位置
	header_len = (unsigned long)(header_end_position - buff);

	char *header = (char *)malloc(header_len);
	len = recv(sock_client, header, header_len, 0);
	// printf("%s:%d len:%lu\n", __FILE__, __LINE__, len);
	if (len == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(-1);
	}
	printf("%s", header);

	char *content_info = strstr(header, "Content-Length:");
	if (content_info == NULL)
	{
		printf("%s:%d Content-Length not match\n", __FILE__, __LINE__);
		return;
	}
	content_info += strlen("Content-Length:");
	content_length = strtoul(content_info, NULL, 10);
	if (content_length == 0)
	{
		printf("%s:%d Content-Length is not a vaild number\n", __FILE__, __LINE__);
		return;
	}

	/*
		recv 要保证接受完所有的数据,目前只有根据 header 中 Content-Length 来计算还未接收的数据
	**/
	char *body = (char *)malloc(content_length);
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
	printf("%s", body);
	puts("");

	free(header);
	free(body);
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
