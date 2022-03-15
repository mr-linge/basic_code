#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

int port = 8000;
char *addr = "127.0.0.1";

int main(int argc,char *argv[])
{
	int sockfd,numbytes;
	char buf[BUFSIZ];
	struct sockaddr_in their_addr;
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
		printf("socket create fail!\n");
		return -1;
	}
	printf("We get the sockfd~\n");
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(port);
	their_addr.sin_addr.s_addr = inet_addr(addr);
	bzero(&(their_addr.sin_zero), 8);

	if(connect(sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr)) == -1) {
		printf("connect fail!\n");
		return -1;
	}
	printf("Get the Server~Cheers!\n");
	numbytes = recv(sockfd, buf, BUFSIZ,0);//接收服务器端信息  
	buf[numbytes]='\0';  
	printf("server response:%s\n",buf);
	while(1)
	{
		printf("input your message:");
		fgets(buf, BUFSIZ, stdin);
		// printf("buf = %s\n",buf);
		if(send(sockfd, buf, strlen(buf) - 1, 0) < 0) {
			perror("write");
			return -1;
		}
		numbytes = recv(sockfd,buf,BUFSIZ,0);  
		buf[numbytes]='\0'; 
		printf("received:%s\n",buf);
		memset(buf,'\0',BUFSIZ);
	}
	close(sockfd);
	
	return 0;
}
