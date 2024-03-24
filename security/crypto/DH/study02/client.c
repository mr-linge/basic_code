#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> // for close
#include <errno.h>
#include <math.h>
#include "dh_exchange.h"
#include "random_prime.h"

#define PORT 8000
#define SERVER_IP "127.0.0.1"


#define LONG_SIZE sizeof(unsigned long)

union PublicKey
{
    unsigned long public_key;
    unsigned char buff[LONG_SIZE];
};

// dh 生成的相同秘钥
char common_key[0x100];

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
		printf("errno:%d,%s\n", errno, strerror(errno));
		exit(-1);
	}
	printf("Connect Server success!\n");

	memset(buff, '\0', BUFSIZ);
    union PublicKey client_publick_key;
    memset(client_publick_key.buff, '\0', LONG_SIZE);
    
	// 生成私钥
    unsigned long client_private_key = generate_random_prime(0x10000000,0x10000000000000);
	//生成需要传送到服务器的key
    client_publick_key.public_key = perform_dh_key_exchange(client_private_key, BASE_GENERATOR);
    printf("client_private_key:%lx,client_publick_key:%lx\n",client_private_key,client_publick_key.public_key);
    memcpy(buff,client_publick_key.buff, LONG_SIZE);
	printf("client_publick_key:");
    for (int i = 0; i < LONG_SIZE; i++)
    {
        printf("%x",client_publick_key.buff[LONG_SIZE - 1 -i]);
    }
    puts("");

	
	// 发送客户端公钥
	if (send(sockfd, client_publick_key.buff, LONG_SIZE, 0) < 0)
	{
		perror("send");
		exit(-1);
	}
	memset(buff, '\0', BUFSIZ);
	numbytes = recv(sockfd, buff, BUFSIZ, 0);
	if (numbytes > 0)
	{
		printf("server len:%02lu msg:", numbytes);
		for (unsigned long i = 0; i < LONG_SIZE; i++)
		{
			unsigned char *tmp = (unsigned char *)buff;
			printf("%02x ", tmp[LONG_SIZE - 1 - i]);
		}
		puts("");

        unsigned long *server_publick_key = (unsigned long *)buff;
        printf("server_publick_key:%lx\n", *server_publick_key);

 		unsigned long tmp = *server_publick_key;
		unsigned long common_secret_key = mod_exp(tmp, client_private_key, PRIME_MODULUS);
		// unsigned long common_secret_key = mod_exp(tmp, client_private_key, PRIME_MODULUS);
        printf("common_secret_key:%lx\n", common_secret_key);
	}
	else
	{
		perror("recv\n");
		exit(-1);
	}

	close(sockfd);
	return 0;
}
