#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8000
int listen_max = 100; // 最大监听数

int main(int argc, char *argv[])
{
	char buff[BUFSIZ];
	int sockfd, client_fd;
	unsigned int struct_len;
	unsigned long numbytes;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY; // inet_addr("xx.xx.xx.xx"); INADDR_ANY
	memset(&(server_addr.sin_zero), '\0', sizeof(server_addr.sin_zero));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("socket create fail");
		exit(-1);
	}
	int ret;
	do
	{
		ret = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
	} while (ret != -1);

	if (listen(sockfd, listen_max) == -1)
	{ // 监听 port
		perror("listen fail");
		exit(-1);
	}
loop:
	printf("Ready for Accept,Waitting...\n");
	client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &struct_len);
	printf("Client was accepting\n");

	while (1)
	{
		memset(buff, '\0', BUFSIZ);
		numbytes = recv(client_fd, buff, BUFSIZ, 0);
		if (numbytes > 0)
		{
			printf("client len:%02lu msg:", numbytes);
			for (unsigned long i = 0; i < numbytes; i++)
			{
				printf("%c", buff[i]);
			}
			printf("\n");
			char *suffix = " | your message was received!";
			memcpy(buff + numbytes, suffix, strlen(suffix));
			unsigned long msg_len = (numbytes + strlen(suffix));
			printf("server len:%02lu msg:%s\n", msg_len, buff);
			if (send(client_fd, buff, msg_len, 0) < 0)
			{
				perror("write");
				break;
			}
			memset(buff, '\0', BUFSIZ);
		}
		else
		{
			perror("recv");
			break;
		}
	}

	close(client_fd);
	goto loop;
	// close(sockfd);
	return 0;
}
