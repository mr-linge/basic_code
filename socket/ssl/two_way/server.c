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
	ctx = SSL_CTX_new(SSLv23_server_method());
	/* 也可以用 SSLv2_server_method() 或 SSLv3_server_method() 单独表示 V2 或 V3标准 */
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
	/* 载入服务器数字证书， 此证书用来发送给客户端,client 会验证这份证书。 证书里包含有公钥 */
	if (SSL_CTX_use_certificate_file(ctx, "../certificate/server.crt", SSL_FILETYPE_PEM) <= 0) { // ./cert/server.cer
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	/* 载入证书私钥 */
	if (SSL_CTX_use_PrivateKey_file(ctx, "../certificate/server_rsa_private.pem.unsecure", SSL_FILETYPE_PEM) <= 0) { // ./cert/server.key
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
	else {
		printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
		ShowCerts(ssl);
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
