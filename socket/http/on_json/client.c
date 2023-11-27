#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include "cJSON.h"

char *ip = "127.0.0.1";
#define PORT 8888

char *method_type = "POST /user HTTP/1.1";
char *client = "Android 8.1";
char *token = "0eefffb6-32af-4fed-833c-866af540akdn";
char *host = "jobs8.cn";

char *make_post_info()
{
	cJSON *json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "key", 1234567);
	cJSON_AddStringToObject(json, "Name", "Li Zhendong");
	cJSON_AddStringToObject(json, "address", "Hubei");

	char *content = cJSON_PrintUnformatted(json);

	char *info = (char *)malloc(0x4000);
	memset(info, 0, 0x4000);
	sprintf(info, "%s\r\n", method_type);
	sprintf(info, "%sUser-Agent: %s\r\n", info, client);
	sprintf(info, "%sAccept: */*\r\n", info);
	sprintf(info, "%sToken: %s\r\n", info, token);
	sprintf(info, "%sHost: %s\r\n", info, host);
	sprintf(info, "%sAccept-Encoding: gzip, deflate, br\r\n", info);
	sprintf(info, "%sConnection: keep-alive\r\n", info);
	sprintf(info, "%sContent-Type: %s\r\n", info, "application/json");
	sprintf(info, "%sContent-Length: %lu\r\n\r\n", info, strlen(content));
	sprintf(info, "%s%s", info, content);

	return info;
}

int main(int argc, char *argv[])
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = inet_addr(ip);
	int result = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
	if (result == -1)
	{
		perror("oops: client connect error");
		return 1;
	}
	char *msg = make_post_info();
	send(sockfd, msg, strlen(msg), 0);
	char buff[BUFSIZ];
	int len = recv(sockfd, buff, BUFSIZ, 0);
	printf("%s\n", buff);
	close(sockfd);

	free(msg);
	return 0;
}
