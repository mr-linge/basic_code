#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 9000
#define SERVER_IP "127.0.0.1"

const char *method_type = "POST /user HTTP/1.1";
const char *client = "Android 8.1";
const char *token = "0eefffb6-32af-4fed-833c-866af540akdn";
const char *host = "jobs8.cn";
const char *content_type = "application/octet-stream";

const char *file_path = "./test.ipa";

void send_http_header(int sock_client, unsigned long http_body_length)
{
	char http_header[BUFSIZ] = {0};

	sprintf(http_header, "%s\r\n", method_type);
	sprintf(http_header, "%sUser-Agent: %s\r\n", http_header, client);
	strcat(http_header, "Accept: */*\r\n");
	sprintf(http_header, "%sToken: %s\r\n", http_header, token);
	sprintf(http_header, "%sHost: %s\r\n", http_header, host);
	strcat(http_header, "Accept-Encoding: gzip, deflate, br\r\n");
	strcat(http_header, "Connection: keep-alive\r\n");
	if (http_body_length > 0)
	{
		sprintf(http_header, "%sContent-Length: %lu\r\n", http_header, http_body_length);
	}
	sprintf(http_header, "%sContent-Type: %s\r\n", http_header, content_type);
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
	unsigned long i, len, write_len = 0;
	len = recv(sockfd, buff, BUFSIZ, 0); // 接收的第一波数据,包含 http header 和 http body, 需要识别并分离第一波数据
	if (len == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		return;
	}
	char *body = strstr(buff, "\r\n\r\n"); // http header 和 http body 以 \r\n\r\n 作为分割
	if (body == NULL)
	{
		puts("http body not find");
		return;
	}
	body += strlen("\r\n\r\n");

	char header[BUFSIZ] = {0};
	memcpy(header, buff, (unsigned long)(body - buff));
	for (i = 0; i < strlen(header); i++)
	{
		printf("%c", buff[i]);
	}

	// 准备把接收的数据写到本地文件里
	int fd = open(file_path, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(-1);
	}

	write_len = write(fd, body, len - strlen(header));
	if (write_len == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(-1);
	}

	memset(buff, '\0', BUFSIZ);
	// 如果 client 发送的数据不止一波,下面就是循环接受完以后所有的数据
	while ((len = recv(sockfd, buff, BUFSIZ, 0)) > 0) // 接收 客户端 发送过来的数据,要确保数据接受完
	{
		if (len == -1)
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			break;
		}
		write_len = write(fd, buff, len);
		if (write_len == -1)
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(-1);
		}
		memset(buff, '\0', BUFSIZ);
		if (len != BUFSIZ) // 当实际读取的字节数 不等于 指定读取的字节数时 意味着数据已经读完了
		{
			break;
		}
	}
	close(fd);
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
