#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> // for close

#define PORT 8000
#define SERVER_IP "127.0.0.1"

int main(int argc, char *argv[])
{
	char buff[BUFSIZ];
	int sockfd;
	unsigned long numbytes;
	struct sockaddr_in server_addr;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket fd create fail");
		exit(-1);
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	memset(&(server_addr.sin_zero), '\0', sizeof(server_addr.sin_zero));

	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(-1);
	}
	printf("Connect Server success!\n");

	memset(buff, '\0', BUFSIZ);
	printf("input your message:");
	fgets(buff, BUFSIZ, stdin);
	buff[strlen(buff) - 1] = '\0'; // 去除输入的 \n
	printf("client len:%02lu msg:%s\n", strlen(buff), buff);
	if (send(sockfd, buff, strlen(buff), 0) == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(-1);
	}
	memset(buff, '\0', BUFSIZ);
	numbytes = recv(sockfd, buff, BUFSIZ, 0);
	if (numbytes > 0)
	{
		printf("server len:%02lu msg:", numbytes);
		for (unsigned long i = 0; i < numbytes; i++)
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
	return 0;
}
