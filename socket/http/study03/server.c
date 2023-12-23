#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h> // read write
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h> // socket
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 9000
int listen_max = 10; // 最大并发数量

const char *file_path = "/tmp/test.ipa";
const char *file_name = "test.ipa";

const char *content_type = "application/json";

void send_http_header(int sock_client, unsigned long http_body_length)
{
	char http_header[BUFSIZ] = {0};

	strcat(http_header, "HTTP/1.1 200 OK\r\n");
	strcat(http_header, "Server: Apache Server V1.0\r\n");
	strcat(http_header, "Accept-Ranges: bytes\r\n");
	strcat(http_header, "Connection: close\r\n");
	if (http_body_length > 0)
	{
		sprintf(http_header, "%sContent-Length: %lu\r\n", http_header, http_body_length);
	}
	sprintf(http_header, "%sContent-Disposition: attachment; filename=%s\r\n", http_header, file_name);
	sprintf(http_header, "%sContent-Type: %s\r\n", http_header, content_type);

	strcat(http_header, "\r\n"); // \r\n 空行后是 body 数据

	if (send(sock_client, http_header, strlen(http_header), 0) == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
	}
}

void send_http_body(int sock_client)
{
	char buff[BUFSIZ] = {0};
	int fd = open(file_path, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(-1);
	}

	unsigned long len = 0;
	// 循环将文件 fd 中的内容读取到 buff 中
	while ((len = read(fd, buff, BUFSIZ)) != 0)
	{
		if (len == -1) // I/O 错误
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			break;
		}
		if (send(sock_client, buff, len, 0) == -1)
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(-1);
		}
		memset(buff, '\0', BUFSIZ);
	}

	close(fd);
}

void http_response(int sock_client)
{
	struct stat buf;
	int fd, ret;
	fd = open(file_path, O_RDONLY);
	ret = fstat(fd, &buf);
	if (ret != 0)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
	}
	printf("%s file size : %llu\n", file_path, buf.st_size);

	send_http_header(sock_client, buf.st_size);
	send_http_body(sock_client);
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

	char buff[BUFSIZ] = {0};
	unsigned long i, len = 0;
	int sock_client = 0;
	while (1)
	{
		sock_client = accept(sockfd, (struct sockaddr *)&claddr, &length);
		if (sock_client < 0)
		{
			fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
			break;
		}
		memset(buff, '\0', BUFSIZ);

		while ((len = recv(sock_client, buff, BUFSIZ, 0)) > 0) // 接收 客户端 发送过来的数据,要确保数据接受完
		{
			if (len == -1)
			{
				fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
				break;
			}
			for (i = 0; i < len; i++)
			{
				printf("%c", buff[i]);
			}
			printf("\n");
			if (len != BUFSIZ) // 当实际读取的字节数 不等于 指定读取的字节数时 意味着数据已经读完了
			{
				break;
			}
		}

		http_response(sock_client);
		close(sock_client);
	}

	close(sockfd);

	return 0;
}
