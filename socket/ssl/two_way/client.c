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
#include <openssl/ssl.h>
#include <openssl/err.h>

int port = 8000;
char *addr = "127.0.0.1";


void ShowCerts(SSL * ssl)
{
	X509 *cert;
	char *line;

	// SSL_get_verify_result()是重点，SSL_CTX_set_verify()只是配置启不启用并没有执行认证，调用该函数才会真证进行证书认证
	// 如果验证不通过，那么程序抛出异常中止连接
	if(SSL_get_verify_result(ssl) == X509_V_OK){
		printf("证书验证通过\n");
	}

	cert = SSL_get_peer_certificate(ssl);
	if (cert != NULL) {
		printf("数字证书信息:\n");
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		printf("证书: %s\n", line);
		free(line);
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		printf("颁发者: %s\n", line);
		free(line);
		X509_free(cert);
	} else
		printf("无证书信息！\n");
}

SSL_CTX * ssl_init() {
	SSL_CTX *ctx;
	/* SSL 库初始化 */
	SSL_library_init();
	/* 载入所有 SSL 算法 */
	OpenSSL_add_all_algorithms();
	/* 载入所有 SSL 错误消息 */
	SSL_load_error_strings();
	/* 以 SSL V2 和 V3 标准兼容方式产生一个 SSL_CTX ，即 SSL Content Text */
	ctx = SSL_CTX_new(SSLv23_client_method());
	if (ctx == NULL) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	SSL_CTX_set_default_passwd_cb_userdata(ctx,"1qaz2wsx");

	// 双向验证
	// SSL_VERIFY_PEER---要求对证书进行认证，没有证书也会放行
	// SSL_VERIFY_FAIL_IF_NO_PEER_CERT---要求客户端需要提供证书，但验证发现单独使用没有证书也会放行
	SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
	// 设置信任根证书
	if (SSL_CTX_load_verify_locations(ctx, "../certificate/ca.crt",NULL)<=0){
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	/* 载入client数字证书， 此证书用来发送给server, server会验证这份证书。 证书里包含有公钥 */
	if (SSL_CTX_use_certificate_file(ctx, "../certificate/client.crt", SSL_FILETYPE_PEM) <= 0) { // ./cert/client.cer
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	/* 载入证书私钥 */
	if (SSL_CTX_use_PrivateKey_file(ctx, "../certificate/client_rsa_private.pem.unsecure", SSL_FILETYPE_PEM) <= 0) { // ./cert/client.key
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
	bzero(&(their_addr.sin_zero), sizeof(their_addr.sin_zero));

	if(connect(sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr)) == -1) {
		printf("connect fail!\n");
		return -1;
	}
	printf("Get the Server~Cheers!\n");

	SSL_CTX *ctx = ssl_init();
	/* 基于 ctx 产生一个新的 SSL */
	SSL *ssl = SSL_new(ctx);
	/* 将连接用户的 socket 加入到 SSL */
	SSL_set_fd(ssl, sockfd);
	/* 建立 SSL 连接 */
	if (SSL_connect(ssl) == -1)
		ERR_print_errors_fp(stderr);
	else {
		printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
		ShowCerts(ssl);
	}

	numbytes = SSL_read(ssl, buf, BUFSIZ);//接收服务器端信息  
	buf[numbytes]='\0';  
	printf("server response:%s\n",buf);
	while(1)
	{
		printf("input your message:");
		fgets(buf, BUFSIZ, stdin);
		// printf("buf = %s\n",buf);
		if(SSL_write(ssl, buf, strlen(buf) - 1) < 0) {
			perror("write");
			return -1;
		}
		numbytes = SSL_read(ssl,buf,BUFSIZ);  
		buf[numbytes]='\0'; 
		printf("received:%s\n",buf);
		memset(buf,0,BUFSIZ);
	}

	close(sockfd);
	/* 关闭连接 */
	SSL_shutdown(ssl);
	SSL_free(ssl);
	SSL_CTX_free(ctx);

	return 0;
}
