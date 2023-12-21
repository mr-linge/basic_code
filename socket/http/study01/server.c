#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h> // read write
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h> // socket
#include <sys/shm.h>

#define PORT 8888
int listen_max = 20; // 最大并发数量

char *insert_http_header(char *http_body)
{
	char *http_response = (char *)malloc(BUFSIZ);
	memset(http_response, '\0', BUFSIZ);

	sprintf(http_response, "HTTP/1.1 200 OK\r\n");
	sprintf(http_response, "%sServer: Apache Server V1.0\r\n", http_response);
	sprintf(http_response, "%sAccept-Ranges: bytes\r\n", http_response);
	sprintf(http_response, "%sConnection: close\r\n", http_response);
	sprintf(http_response, "%sContent-Length: %lu\r\n", http_response, strlen(http_body));
	sprintf(http_response, "%sContent-Type: %s\r\n\r\n", http_response, "application/json"); // \r\n 换行后是 body

	sprintf(http_response, "%s%s", http_response, http_body);

	return http_response;
}

void http_response(int sock_client, char *buff, int len)
{
	printf("%s\n", buff);
	char *http_body = NULL;
	char *p = strstr(buff, "Content-Length:");
	if (p != NULL)
	{
		p += strlen("Content-Length:");

		unsigned long val = strtoul(p, NULL, 10);
		if (val == 0)
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			return;
		}
		char content[val + 1];
		memset(content, 0, val + 1);
		memcpy(content, buff + (len - val), val);
		// printf("request length:%lu body:\n%s\n", val, content);
		http_body = "{\"code\":200,\"msg\":\"success\",\"data\":\"you will be success!\"}";
	}
	else
	{
		http_body = "Client Http body is empty.";
	}

	char *http_response_content = insert_http_header(http_body);
	if (send(sock_client, http_response_content, strlen(http_response_content), 0) < 0)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
	}
	free(http_response_content);
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
	char buff[BUFSIZ] = {0};
	struct sockaddr_in claddr;
	socklen_t length = sizeof(claddr);

	while (1)
	{
		int sock_client = accept(sockfd, (struct sockaddr *)&claddr, &length);
		if (sock_client < 0)
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			close(sock_client);
			break;
		}
		memset(buff, '\0', sizeof(buff));
		int len = recv(sock_client, buff, sizeof(buff), 0);
		// printf("recv len:%d\n", len);
		if (len < 0)
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			close(sock_client);
			break;
		}

		http_response(sock_client, buff, len);
		close(sock_client);
	}

	close(sockfd);

	return 0;
}
