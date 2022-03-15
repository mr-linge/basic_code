#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

#define SERVER_PORT 8888
#define BUFF_LEN 1024

void handle_udp_msg(int fd)
{
	char *msg = "This is a message from server.";
	char buf[BUFF_LEN];  //接收缓冲区，1024字节
	int count;
	struct sockaddr_in client_addr;  //clent_addr用于记录发送方的地址信息
	socklen_t len = sizeof(client_addr);
	while(1)
	{
		memset(buf, 0, BUFF_LEN);
		count = recvfrom(fd, buf, BUFF_LEN, 0, (struct sockaddr*)&client_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞
		if(count == -1)
		{
			printf("recieve data fail!\n");
			return;
		}
		printf("client:%s\n",buf);  //打印client发过来的信息
		sendto(fd, msg, strlen(msg), 0, (struct sockaddr*)&client_addr, len);  //发送信息给client，注意使用了clent_addr结构体指针
	}
}


int main(int argc, char* argv[])
{
	int server_fd, ret;

	server_fd = socket(AF_INET, SOCK_DGRAM, 0); //AF_INET:IPV4;SOCK_DGRAM:UDP
	if(server_fd < 0)
	{
		printf("create socket fail!\n");
		return -1;
	}

	struct sockaddr_in ser_addr;
	memset(&ser_addr, 0, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址，需要进行网络序转换，INADDR_ANY：本地地址
	ser_addr.sin_port = htons(SERVER_PORT);  //端口号，需要网络序转换

	ret = bind(server_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
	if(ret < 0)
	{
		printf("socket bind fail!\n");
		return -1;
	}

	handle_udp_msg(server_fd);   //处理接收到的数据

	close(server_fd);
	return 0;
}
