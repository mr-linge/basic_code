#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define SERVER_PORT 8888
#define SERVER_IP "127.0.0.1"
#define BUFF_LEN 1024


void udp_msg_sender(int fd, struct sockaddr* dst)
{
	char *msg = "Hello,I am client.";
	char buf[BUFF_LEN];
	struct sockaddr_in server_client;
	socklen_t len = sizeof(*dst);
	while(1)
	{
		sendto(fd, msg, strlen(msg), 0, dst, len);
		memset(buf, 0, BUFF_LEN);
		recvfrom(fd, buf, BUFF_LEN, 0, (struct sockaddr*)&server_client, &len);  //接收来自server的信息
		printf("server:%s\n",buf);
		sleep(5);  //一秒发送一次消息
	}
}

int main(int argc, char* argv[])
{
	int client_fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(client_fd < 0)
	{
		printf("create socket fail!\n");
		return -1;
	}

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  //注意网络序转换
	server_addr.sin_port = htons(SERVER_PORT);  //注意网络序转换

	udp_msg_sender(client_fd, (struct sockaddr*)&server_addr);

	close(client_fd);

	return 0;
}
