#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>	// for open
#include <unistd.h> // for close

int port = 8000;
char *addr = "127.0.0.1";

int main(int argc, char *argv[])
{
	int sockfd;
	unsigned long numbytes;
	char buff[BUFSIZ];
	bzero(buff, BUFSIZ);
	struct sockaddr_in server_addr;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("socket create fail!\n");
		return -1;
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(addr);
	bzero(&(server_addr.sin_zero), sizeof(server_addr.sin_zero));

	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		printf("connect fail!\n");
		return -1;
	}
	printf("Connect Server success!\n");

	while (1)
	{
		printf("input your message:");
		fgets(buff, BUFSIZ, stdin);
		if (send(sockfd, buff, strlen(buff) - 1, 0) < 0)
		{
			perror("socket send");
			return -1;
		}
		bzero(buff, BUFSIZ);
		numbytes = recv(sockfd, buff, BUFSIZ, 0);
		if (numbytes)
		{
			buff[numbytes] = '\0';
			printf("From Server:\n%s\n", buff);
			bzero(buff, BUFSIZ);
		}
		else
		{
			printf("Server accepting close!\n");
			close(sockfd);
			break;
		}
	}

	return 0;
}
