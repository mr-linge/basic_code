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

const char *file_path = "./test.ipa";

void send_http_header(int sock_client, unsigned long http_body_length)
{
	char http_header[BUFSIZ] = {0};

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
	sprintf(http_header, "%sContent-Type: %s\r\n", http_header, "application/octet-stream");
	strcat(http_header, "\r\n"); // \r\n 空行后是 body 数据

	if (send(sock_client, http_header, strlen(http_header), 0) == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
	}
}

void send_http_body(int sock_client, char *http_body, unsigned long len)
{

	if (send(sock_client, http_body, len, 0) == -1)
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

void handle_response(int sockfd)
{
	char buff[BUFSIZ] = {0};

	unsigned long len = 0;
	unsigned long body_length = 0;
	len = recv(sockfd, buff, BUFSIZ, 0); // 读取时遇到 \r\n(空行)就完成一次读入。读取 http 协议数据, 第一次读取到的是 header
	printf("%s", buff);
	if (len > 0 && len <= BUFSIZ)
	{
		char *p = strstr(buff, "Content-Length:");
		if (p == NULL)
		{
			printf("http response body is empty.\n");
			return;
		}
		p += strlen("Content-Length:");
		body_length = strtoul(p, NULL, 10);
		printf("body_length:%lu\n", body_length);
		if (body_length == 0)
		{
			printf("http response body is empty.\n");
			return;
		}
	}
	else
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		return;
	}

	FILE *fp = fopen(file_path, "ab+");
	if (fp == NULL)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(-1);
	}
	unsigned long size_count = 0; // 写入文件时每次写入的 size count
	while (body_length > 0)
	{
		memset(buff, '\0', BUFSIZ);
		len = recv(sockfd, buff, BUFSIZ, 0);
		if (len > 0 && len <= BUFSIZ)
		{
			size_count = fwrite(buff, sizeof(char), len, fp); // 将读取的文件写入到另一个地方 ./test.ipa
			if (size_count != len && ferror(fp))
			{
				printf("写入文件:%s 时发生错误\n", file_path);
				break;
			}
			clearerr(fp);
			printf("%s:%d body_length:%lu len:%lu\n", __FILE__, __LINE__, body_length, len);
			body_length -= len;
		}
		else
		{
			printf("%s:%d len:%lu\n", __FILE__, __LINE__, len);
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			break;
		}
	}
	printf("%s:%d body_length:%lu\n", __FILE__, __LINE__, body_length);
	fclose(fp);
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
	// 处理接收到的数据
	handle_response(sockfd);

	close(sockfd);
	return 0;
}
