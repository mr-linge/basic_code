#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8888
#define SERVER_IP "127.0.0.1"

char *method_type = "POST /user HTTP/1.1";
char *client = "Android 8.1";
char *token = "0eefffb6-32af-4fed-833c-866af540akdn";
char *host = "jobs8.cn";

char *insert_http_header(char *http_body)
{
	char *http_response = (char *)malloc(BUFSIZ);
	memset(http_response, '\0', BUFSIZ);
	sprintf(http_response, "%s\r\n", method_type);
	sprintf(http_response, "%sUser-Agent: %s\r\n", http_response, client);
	sprintf(http_response, "%sAccept: */*\r\n", http_response);
	sprintf(http_response, "%sToken: %s\r\n", http_response, token);
	sprintf(http_response, "%sHost: %s\r\n", http_response, host);
	sprintf(http_response, "%sAccept-Encoding: gzip, deflate, br\r\n", http_response);
	sprintf(http_response, "%sConnection: keep-alive\r\n", http_response);
	sprintf(http_response, "%sContent-Length: %lu\r\n", http_response, strlen(http_body));
	sprintf(http_response, "%sContent-Type: %s\r\n\r\n", http_response, "application/json"); // http header 和 body 以空行为分隔 \r\n 换行后是 body
	sprintf(http_response, "%s%s", http_response, http_body);

	return http_response;
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
	char *http_response_content = insert_http_header(http_body);
	if (send(sockfd, http_response_content, strlen(http_response_content), 0) < 0)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	
	char buff[BUFSIZ] = {0};
	unsigned long len = recv(sockfd, buff, BUFSIZ, 0);
	if (len > 0)
	{
		// printf("received data len:%02lu msg:", len);
		for (unsigned long i = 0; i < len; i++)
		{
			printf("%c", buff[i]);
		}
		printf("\n");
	}
	else
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
	}

	close(sockfd);
	free(http_response_content);
	return 0;
}
