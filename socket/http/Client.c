#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>


char * ip = "127.0.0.1";
#define PORT 8888

char * CLIENT_INFO = "%s\r\n"
		     "User-Agent: Android 8.1\r\n"
	             "Accept: */*\r\n"
	             "Token: %s\r\n"
	             "Host: %s\r\n"
	             "Accept-Encoding: gzip, deflate, br\r\n"
	             "Connection: keep-alive\r\n";

int make_get_info(char * http_info) {
	char * method_type = "GET /user HTTP/1.1";
	char * token = "0eefffb6-32af-4fed-833c-866af540a5d4";
	char * host = "jobs8.cn";
	int http_info_len = sprintf(http_info, CLIENT_INFO, method_type, token, host);
	
	return http_info_len;
}

int make_post_info(char * http_info) {
	char * method_type = "POST /user HTTP/1.1";
	char * token = "0eefffb6-32af-4fed-833c-866af540akdn";
	char * host = "jobs8.cn";
	char http_header[0x1000];
	sprintf(http_header,CLIENT_INFO,method_type, token, host);

	char * content_info = "Content-Type: %s\r\n"
			      "Content-Length: %d\r\n\r\n%s";
	char * content_type = "application/json";
	char * content_text = "{\"a\":\"A\",\"b\":\"B\",\"c\":\"C\"}";
	char content[0x1000];
	sprintf(content,content_info,content_type,strlen(content_text),content_text);
	
	int http_info_len = sprintf(http_info, "%s%s", http_header, content);
	
	return http_info_len;
}

int main(int argc, char *argv[]) {
	char HTTP_INFO[0x1000]; 
	int http_info_len = make_get_info(HTTP_INFO);
	//int http_info_len = make_post_info(HTTP_INFO);
	printf("%s\n", HTTP_INFO);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	struct sockaddr_in address; 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = inet_addr(ip); 
	address.sin_port = htons(PORT); 
	int result = connect(sockfd,(struct sockaddr *)&address,sizeof(address)); 
	if(result == -1) {
		perror("oops: client connect error");
		return 1;
	}
	send(sockfd,HTTP_INFO,http_info_len,0);
	char buff[0x1000];
	int len = recv(sockfd, buff, sizeof(buff), 0);
	printf("%s\n", buff);
	close(sockfd);

	return 0;
} 
