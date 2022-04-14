#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h> // socket
#include <sys/types.h>  // 基本数据类型
#include <unistd.h>     // read write
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> // open close
#include <sys/shm.h>

#define PORT 8888
#define BACKLOG 20 // 最大并发数量

void response(int sock_client,char *content);
void parse_request(char *buff,int sock_client);

int main() {
	int count = 0; // 计数
	// 定义 socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// 定义 sockaddr_in
	struct sockaddr_in skaddr;
	skaddr.sin_family = AF_INET; // ipv4
	skaddr.sin_port = htons(PORT);
	skaddr.sin_addr.s_addr = INADDR_ANY;
	// bind，绑定 socket 和 sockaddr_in
	if (bind(sockfd, (struct sockaddr *)&skaddr, sizeof(skaddr)) == -1)
	{
		perror("bind error");
		exit(1);
	}

	// listen，开始添加端口
	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen error");
		exit(1);
	}

	// 客户端信息
	char buff[BUFSIZ];
	struct sockaddr_in claddr;
	socklen_t length = sizeof(claddr);

	while (1)
	{
		int sock_client = accept(sockfd, (struct sockaddr *)&claddr, &length);
		printf("%d\n", ++count);
		if (sock_client < 0)
		{
			perror("accept error");
			exit(1);
		}
		memset(buff, 0, sizeof(buff));
		recv(sock_client, buff, sizeof(buff), 0);

		parse_request(buff,sock_client);
		close(sock_client);
	}

	close(sockfd);

	return 0;
}

void parse_request(char *buff,int sock_client) {
	printf("%s\n", buff);

	char *p = strstr(buff,"Content-Length:");
	p += strlen("Content-Length:");

	unsigned long val = strtoul(p, NULL, 10);
	if (val == 0) {
		puts("strtoul error");
		exit(-1);
	}
	//printf("val = %lu\n", val);
	char content[val + 1];
	memset(content,0,val + 1);
	memcpy(content,buff + (strlen(buff) - val),val);
	//printf("len:%lu content:%s\n",strlen(content) ,content);	

	char *msg = "{\"code\":200,\"msg\":\"success\",\"data\":\"you will be success!\"}";
	response(sock_client, msg);
}

void response(int sock_client,char *content) {
	char info[BUFSIZ];
	sprintf(info,"HTTP/1.1 200 OK\r\n");
	sprintf(info,"%sServer: Apache Server V1.0\r\n",info);
	sprintf(info,"%sAccept-Ranges: bytes\r\n",info);
	sprintf(info,"%sContent-Length: %lu\r\n",info,strlen(content));
	sprintf(info,"%sConnection: close\r\n",info);
	sprintf(info,"%sContent-Type: %s\r\n\r\n",info,"application/json");

	sprintf(info,"%s%s",info,content);

	send(sock_client, info, strlen(info), 0);
}
