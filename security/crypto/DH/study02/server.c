#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <math.h>
#include "dh_exchange.h"
#include "random_prime.h"

#define PORT 8000
int listen_max = 100; // 最大监听数

#define LONG_SIZE sizeof(unsigned long)

union PublicKey
{
    unsigned long public_key;
    unsigned char buff[LONG_SIZE];
};

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

	// while (1)
	// {
		memset(buff, '\0', BUFSIZ);
		numbytes = recv(client_fd, buff, BUFSIZ, 0);
		if (numbytes > 0)
		{
			printf("client len:%02lu msg:", numbytes);
			for (unsigned long i = 0; i < numbytes; i++)
			{
                unsigned char *tmp = (unsigned char *)buff;
			    printf("%02x ", tmp[LONG_SIZE - 1 - i]);
			}
			puts("");

            unsigned long *client_publick_key = (unsigned long *)buff;
            printf("client_publick_key:%lx\n", *client_publick_key);

            // 生成私钥
            unsigned long server_private_key = generate_random_prime(0x100000000,0x10000000000000); // Bob的私钥

            union PublicKey server_publick_key;
            memset(server_publick_key.buff, '\0', LONG_SIZE);
            //生成需要传送的key
            server_publick_key.public_key = perform_dh_key_exchange(server_private_key, BASE_GENERATOR);//perform_dh_key_exchange(server_private_key, BASE_GENERATOR);
            printf("server_private_key:%lx,server_publick_key:%lx\n",server_private_key,server_publick_key.public_key);

			if (send(client_fd, server_publick_key.buff, LONG_SIZE, 0) < 0)
			{
				perror("write");
				// break;
			}

            unsigned long tmp = *client_publick_key;
            unsigned long common_secret_key = mod_exp(tmp, server_private_key, PRIME_MODULUS);
            printf("common_secret_key:%lx\n", common_secret_key);
		}
		else
		{
			perror("recv");
			// break;
		}
	// }
	
	close(client_fd);
	goto loop;
	// close(sockfd);
	return 0;
}
