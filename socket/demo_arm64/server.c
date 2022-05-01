#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int port = 8000;
int listen_port = 5;	//最大监听数

int main(int argc, char *argv[]) {
	int fd, new_fd, struct_len, numbytes, i;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	char buff[BUFSIZ];

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero), sizeof(server_addr.sin_zero));
	struct_len = sizeof(struct sockaddr_in);

	fd = socket(AF_INET, SOCK_STREAM, 0);
	while(bind(fd, (struct sockaddr *)&server_addr, struct_len) == -1);
	if(listen(fd, listen_port) == -1){
		printf("Listening fail\n");
		return -1;
	}

	printf("Ready for Accept,Waitting...\n");
	new_fd = accept(fd, (struct sockaddr *)&client_addr, &struct_len);
	printf("Get the Client.\n");
	char *msg = "Welcome to my server!";
	if(send(new_fd,msg,strlen(msg),0) < 0) {
		perror("write");
		return 1;
	}
	while((numbytes = recv(new_fd, buff, BUFSIZ, 0)) > 0)
	{
		buff[numbytes] = '\0';
		printf("%s\n",buff);
		if(send(new_fd,buff,numbytes,0) < 0) {
			perror("write");
			return 1;
		}
	}
	close(new_fd);

	close(fd);
	return 0;
}
