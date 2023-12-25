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

void http_response(int sock_client)
{
	char *http_body = "{\"code\":200,\"msg\":\"success\",\"data\":\"you will be success!\"}";
	send_http_header(sock_client, strlen(http_body));
	send_http_body(sock_client, http_body, strlen(http_body));
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
	char buff[BUFSIZ] = {0};
	while (1)
	{
		sock_client = accept(sockfd, (struct sockaddr *)&claddr, &length);
		if (sock_client < 0)
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			close(sock_client);
			exit(-1);
		}
		memset(buff, '\0', BUFSIZ);

		len = recv(sock_client, buff, BUFSIZ, 0); // 接收 客户端 发送过来的数据,目前只接收一次数据(这个缺陷 在 study03 里完善了)
		if (len == -1)
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(-1);
		}
		for (i = 0; i < len; i++)
		{
			printf("%c", buff[i]);
		}
		printf("\n");

		http_response(sock_client);
		close(sock_client);
	}

	close(sock_client);
	close(sockfd);

	return 0;
}
