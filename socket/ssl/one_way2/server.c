#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

int port = 8000;
int listen_port = 10;    //最大监听数
char *addr = "127.0.0.1";			 

SSL_CTX * ssl_init() {
	SSL_CTX *ctx;
	/* SSL 库初始化 */
	SSL_library_init();
	/* 载入所有 SSL 算法 */
	OpenSSL_add_all_algorithms();
	/* 载入所有 SSL 错误消息 */
	SSL_load_error_strings();
	/* 以 SSL V2 和 V3 标准兼容方式产生一个 SSL_CTX ，即 SSL Content Text */
	ctx = SSL_CTX_new(SSLv23_server_method());
	/* 也可以用 SSLv2_server_method() 或 SSLv3_server_method() 单独表示 V2 或 V3标准 */
	if (ctx == NULL) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	/* 载入用户的数字证书， 此证书用来发送给客户端。 证书里包含有公钥 */
	if (SSL_CTX_use_certificate_file(ctx, "./cacert.pem", SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	/* 载入用户私钥 */
	if (SSL_CTX_use_PrivateKey_file(ctx, "./privkey.pem", SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	/* 检查用户私钥是否正确 */
	if (!SSL_CTX_check_private_key(ctx)) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	return ctx;
}

int main(int argc, char **argv) {
	int sockfd, new_fd;
	socklen_t len;
	struct sockaddr_in my_addr, their_addr;
	char buf[BUFSIZ];

	SSL_CTX *ctx = ssl_init();    

	/* 开启一个 socket 监听 */
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	} else {
		printf("socket created\n");
	}
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = PF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	//my_addr.sin_addr.s_addr = inet_addr(addr);

	if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(sockfd, listen_port) == -1) {
		perror("listen");
		exit(1);
	}

	char *msg = "Welcome to my server!";
	while (1) {
		len = sizeof(struct sockaddr);
		/* 等待客户端连上来 */
		if ((new_fd = accept(sockfd, (struct sockaddr *) &their_addr,&len)) == -1) {
			perror("accept");
			exit(errno);
		}
		/* 基于 ctx 产生一个新的 SSL */
		SSL *ssl = SSL_new(ctx);
		/* 将连接用户的 socket 加入到 SSL */
		SSL_set_fd(ssl, new_fd);
		/* 建立 SSL 连接 */
		if (SSL_accept(ssl) == -1) {
			perror("accept");
			close(new_fd);
			break;
		}
		/* 开始处理每个新连接上的数据收发 */
		/* 发消息给客户端 */
		len = SSL_write(ssl, msg, strlen(msg));
		if (len <= 0) {
			printf("消息'%s'发送失败！错误代码是%d，错误信息是'%s'\n",msg, errno, strerror(errno));
			goto finish;
		}

		bzero(buf, BUFSIZ);
		/* 接收客户端的消息 */
		len = SSL_read(ssl, buf, BUFSIZ);
		if (len > 0) {
			printf("接收消息成功:'%s'，共%d个字节的数据\n", buf, len);
		} else {
			printf("消息接收失败！错误代码是%d，错误信息是'%s'\n", errno, strerror(errno));
		}
		/* 处理每个新连接上的数据收发结束 */
finish:
		/* 关闭 SSL 连接 */
		SSL_shutdown(ssl);
		/* 释放 SSL */
		SSL_free(ssl);
		/* 关闭 socket */
		close(new_fd);
	}

	/* 关闭监听的 socket */
	close(sockfd);
	/* 释放 CTX */
	SSL_CTX_free(ctx);

	return 0;
}
