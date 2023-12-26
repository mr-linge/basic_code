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

void insert_http_header(char *http_response, char *body)
{
	char header[BUFSIZ] = {0};

	strcat(header, "HTTP/1.1 200 OK\r\n");
	strcat(header, "Server: Apache Server V1.0\r\n");
	strcat(header, "Accept-Ranges: bytes\r\n");
	strcat(header, "Connection: close\r\n");
	sprintf(header, "%sContent-Length: %lu\r\n", header, strlen(body));
	sprintf(header, "%sContent-Type: %s\r\n", header, content_type);
	strcat(header, "\r\n"); // \r\n 换行后是 body

	sprintf(http_response, "%s%s", header, body);
}

void send_data(int sock_client)
{
	char *http_body = "{\"code\":200,\"msg\":\"success\",\"data\":\"you will be success!\"}";
	char *http_response = (char *)malloc(BUFSIZ);
	memset(http_response, '\0', BUFSIZ);
	insert_http_header(http_response, http_body);
	if (send(sock_client, http_response, strlen(http_response), 0) < 0)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
	}
	free(http_response);
}

void received_data(int sock_client)
{
	char buff[BUFSIZ] = {0};
	unsigned long i, len;
	len = recv(sock_client, buff, sizeof(buff), 0); // 数据量少,一次即可接收完成
	if (len == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(-1);
	}
	for (i = 0; i < len; i++)
	{
		printf("%c", buff[i]);
	}
	puts("");
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
	while (1)
	{
		sock_client = accept(sockfd, (struct sockaddr *)&claddr, &length);
		if (sock_client < 0)
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(-1);
		}

		// 接收数据
		received_data(sock_client);
		// 发送数据
		send_data(sock_client);

		close(sock_client);
	}

	close(sockfd);

	return 0;
}
