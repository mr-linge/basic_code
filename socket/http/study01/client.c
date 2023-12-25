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

const char *method_type = "POST /user HTTP/1.1";
const char *client = "Android 8.1";
const char *token = "0eefffb6-32af-4fed-833c-866af540akdn";
const char *host = "jobs8.cn";
const char *content_type = "application/json";

void insert_http_header(char *http_response, char *body)
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

	sprintf(http_response, "%s%s", header, body);
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

	char *http_body = "{\"key\":\"123456\",\"name\":\"Dio\",\"address\":\"BJ\"}";
	char *http_response = (char *)malloc(BUFSIZ);
	memset(http_response, '\0', BUFSIZ);
	insert_http_header(http_response, http_body);
	if (send(sockfd, http_response, strlen(http_response), 0) < 0)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}

	char buff[BUFSIZ] = {0};
	unsigned long len = recv(sockfd, buff, BUFSIZ, 0);
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
	free(http_response);
	return 0;
}
