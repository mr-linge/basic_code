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

char *method_type = "POST /user HTTP/1.1";
char *client = "Android 8.1";
char *token = "0eefffb6-32af-4fed-833c-866af540akdn";
char *host = "jobs8.cn";

void send_http_header(int sock_client, unsigned long http_body_length)
{
	char *http_header = (char *)malloc(BUFSIZ);
	memset(http_header, '\0', BUFSIZ);

	sprintf(http_header, "%s\r\n", method_type);
	sprintf(http_header, "%sUser-Agent: %s\r\n", http_header, client);
	sprintf(http_header, "%sAccept: */*\r\n", http_header);
	sprintf(http_header, "%sToken: %s\r\n", http_header, token);
	sprintf(http_header, "%sHost: %s\r\n", http_header, host);
	sprintf(http_header, "%sAccept-Encoding: gzip, deflate, br\r\n", http_header);
	sprintf(http_header, "%sConnection: keep-alive\r\n", http_header);
	if (http_body_length > 0)
	{
		sprintf(http_header, "%sContent-Length: %lu\r\n", http_header, http_body_length);
	}
	sprintf(http_header, "%sContent-Type: %s\r\n\r\n", http_header, "application/json"); // http header 和 body 以空行为分隔 \r\n 换行后是 body

	if (send(sock_client, http_header, strlen(http_header), 0) < 0)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
	}
	free(http_header);
}

void send_http_body(int sock_client, char *http_body, unsigned long len)
{
	if (send(sock_client, http_body, len, 0) < 0)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
	}
}

void http_request(int sock_client)
{
	char *http_body = "{\"key\":\"123456\",\"name\":\"Dio\",\"address\":\"BJ\"}";
	send_http_header(sock_client, strlen(http_body));
	send_http_body(sock_client, http_body, strlen(http_body));
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

	// 向服务器发出请求
	http_request(sockfd);

	char buff[BUFSIZ] = {0};
	unsigned long len = recv(sockfd, buff, BUFSIZ, 0); // 目前接收一次数据就能获取到所有client发送的数据
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
	printf("\n");

	close(sockfd);
	return 0;
}
