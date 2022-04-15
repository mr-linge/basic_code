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

#include "cJSON.h"


#define PORT 8888
#define BACKLOG 20 // 最大并发数量

void response(int sock_client,char *content);
void parse_request(char *buff, int len, int sock_client);

int main() {
	// 计数
	int count = 0; 
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
		int len = recv(sock_client, buff, sizeof(buff), 0);

		parse_request(buff,len,sock_client);
		close(sock_client);
	}

	close(sockfd);

	return 0;
}

void parse_request(char *buff, int len, int sock_client) {
	printf("%s\n", buff);

	char *p = strstr(buff,"Content-Length:");
	p += strlen("Content-Length:");

	unsigned long val = strtoul(p, NULL, 10);
	if (val == 0) {
		puts("no content text!");
		return;
	}
	//printf("val = %lu\n", val);
	char content[val + 1];
	memset(content,0,val + 1);
	memcpy(content,buff + (len - val),val);
	//printf("len:%lu content:%s\n",strlen(content) ,content);	

	cJSON * rev_data = cJSON_Parse(content);
	if (rev_data == NULL)return;
	cJSON * key = cJSON_GetObjectItem(rev_data, "key");
	if(key->valueint != 123456) {
		char * tmp = "Your key is wrong!";
		response(sock_client, tmp);
		return;
	}
	cJSON * item = cJSON_GetObjectItem(rev_data, "name");


	//1. 创建cJSON对象
	cJSON * json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "code", 200);
	cJSON_AddStringToObject(json, "msg", "success");
	char tmp[0x100];
	sprintf(tmp,"%s%s",item->valuestring,", Believe you will be what you want to be");
	cJSON_AddStringToObject(json, "data",tmp);

	char *json_str = cJSON_PrintUnformatted(json);
	response(sock_client, json_str);
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
