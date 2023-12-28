#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 9000
#define SERVER_IP "127.0.0.1"

const char *method_type = "POST /get_data HTTP/1.1";
const char *client = "Android 8.1";
const char *token = "0eefffb6-32af-4fed-833c-866af540akdn";
const char *host = "jobs8.cn";
const char *content_type = "application/json";

void insert_http_header(char *http_request, char *body)
{
	char header[BUFSIZ] = {0};

	sprintf(header, "%s\r\n", method_type);
	sprintf(header, "%sUser-Agent: %s\r\n", header, client);
	sprintf(header, "%sAccept: */*\r\n", header);
	sprintf(header, "%sToken: %s\r\n", header, token);
	sprintf(header, "%sHost: %s\r\n", header, host);
	sprintf(header, "%sAccept-Encoding: gzip, deflate, br\r\n", header);
	sprintf(header, "%sConnection: keep-alive\r\n", header);
	sprintf(header, "%sContent-Length: %lu\r\n", header, strlen(body));
	sprintf(header, "%sContent-Type: %s\r\n", header, content_type);

	strcat(header, "\r\n"); // http header 和 body 以空行为分隔 \r\n 换行后是 body

	sprintf(http_request, "%s%s", header, body);
}

void send_data(int sockfd)
{
	char *http_body = "{\"key\":\"123456\",\"name\":\"Dio\",\"address\":\"BJ\"}";
	char http_request[BUFSIZ] = {0};
	insert_http_header(http_request, http_body);
	if (send(sockfd, http_request, strlen(http_request), 0) < 0) // 发送 http 请求,数据量少一次即可发送完
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(-1);
	}
}

void receive_data(int sockfd)
{
	char buff[BUFSIZ] = {0};
	unsigned long len = recv(sockfd, buff, BUFSIZ, 0); // 接收响应数据,数据量少一次即可接收完
	if (len == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(-1);
	}
	// printf("received data len:%02lu msg:", len);
	for (unsigned long i = 0; i < len; i++)
	{
		printf("%c", buff[i]);
	}
	puts("");
}

int main(int argc, char *argv[])
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = inet_addr(SERVER_IP);
	int result = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
	if (result == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}

	// 发送数据
	send_data(sockfd);
	// 接收数据
	receive_data(sockfd);

	close(sockfd);
	return 0;
}
