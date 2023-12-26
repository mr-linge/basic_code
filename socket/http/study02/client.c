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

void send_data(int sock_client)
{
	char *http_body = "{\"key\":\"123456\",\"name\":\"Dio\",\"address\":\"BJ\"}";
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
	send_data(sockfd);

	// 接收传送过来的 http 数据
	receive_data(sockfd);

	close(sockfd);
	return 0;
}
