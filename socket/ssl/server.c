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
#include <openssl/ssl.h>
#include <openssl/err.h>

int port = 8000;
int listen_port = 5;	//最大监听数

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

int main(int argc, char *argv[]) {
	int fd, new_fd, numbytes;
	socklen_t struct_len;
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
	new_fd = accept(fd, (struct sockaddr *)&client_addr, (socklen_t *)&struct_len);
	printf("Get the Client.\n");

	SSL_CTX *ctx = ssl_init();
	/* 基于 ctx 产生一个新的 SSL */
	SSL *ssl = SSL_new(ctx);
	/* 将连接用户的 socket 加入到 SSL */
	SSL_set_fd(ssl, new_fd);
	/* 建立 SSL 连接 */
	if (SSL_accept(ssl) == -1) {
		perror("accept");
		close(new_fd);
		return -1;
	}
	char *msg = "Welcome to my server!";
	// send message to client
	if(SSL_write(ssl,msg,strlen(msg)) < 0) {
		perror("write");
		return 1;
	}
	while((numbytes = SSL_read(ssl, buff, BUFSIZ)) > 0)
	{
		buff[numbytes] = '\0';
		printf("%s\n",buff);
		if(SSL_write(ssl,buff,numbytes) < 0) {
			perror("write");
			return 1;
		}
	}

	close(new_fd);
	close(fd);
	/* 关闭连接 */
	SSL_shutdown(ssl);
	SSL_free(ssl);
	SSL_CTX_free(ctx);

	return 0;
}
