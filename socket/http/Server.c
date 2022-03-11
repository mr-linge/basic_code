#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h> // socket
#include <sys/types.h>  // 基本数据类型
#include <unistd.h> // read write
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> // open close
#include <sys/shm.h>

#include <signal.h>

#define PORT 8888
#define SERV "0.0.0.0"
#define QUEUE 20
#define BUFF_SIZE 1024


typedef struct doc_type{
        char *key;
        char *value;
}HTTP_CONTENT_TYPE;

HTTP_CONTENT_TYPE http_content_type[] = {
        { "html","text/html" },
        { "gif" ,"image/gif" },
        { "jpeg","image/jpeg" }
};

int sockfd;
char *http_res_tmpl = "HTTP/1.1 200 OK\r\n"
        "Server: Cleey's Server V1.0\r\n"
    "Accept-Ranges: bytes\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "Content-Type: %s\r\n\r\n";

void handle_signal(int sign); // 退出信号处理
void http_send(int sock,char *content); // http 发送相应报文
char* joinString(char *s1, char *s2);//字符串拼接

int main(int argc,char *argv[ ]){

        signal(SIGINT,handle_signal);
        int count = 0; // 计数
        // 定义 socket
        sockfd = socket(AF_INET,SOCK_STREAM,0);
        // 定义 sockaddr_in
        struct sockaddr_in skaddr;
        skaddr.sin_family = AF_INET; // ipv4
        skaddr.sin_port   = htons(PORT);
        skaddr.sin_addr.s_addr = inet_addr(SERV);
        // bind，绑定 socket 和 sockaddr_in
        if(bind(sockfd,(struct sockaddr *)&skaddr,sizeof(skaddr)) == -1){
                perror("bind error");
                exit(1);
        }

        // listen，开始添加端口
        if(listen(sockfd,QUEUE) == -1){
                perror("listen error");
                exit(1);
        }

        // 客户端信息
        char buff[BUFF_SIZE];
        struct sockaddr_in claddr;
        socklen_t length = sizeof(claddr);


        while(1){
                int sock_client = accept(sockfd,(struct sockaddr *)&claddr, &length);
                printf("%d\n",++count);
                if( sock_client <0 ){
                        perror("accept error");
                        exit(1);
                }
                memset(buff,0,sizeof(buff));
                int len = recv(sock_client,buff,sizeof(buff),0);
                fputs(buff,stdout);
                //send(sock_client,buff,len,0);
                char *re = joinString("<h2>the client said</h2> <br>  ", buff);
                http_send(sock_client,re);
                close(sock_client);
        }
        fputs("Bye Cleey",stdout);
        close(sockfd);
        return 0;
}

void http_send(int sock_client,char *content){
        char HTTP_HEADER[BUFF_SIZE],HTTP_INFO[BUFF_SIZE];
        int len = strlen(content);
        sprintf(HTTP_HEADER,http_res_tmpl,len,"text/html");
        len = sprintf(HTTP_INFO,"%s%s",HTTP_HEADER,content);

        send(sock_client,HTTP_INFO,len,0);
}

void handle_signal(int sign){
        fputs("\nSIGNAL INTERRUPT \nBye Cleey! \nSAFE EXIT\n",stdout);
        close(sockfd);
        exit(0);
}

char* joinString(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    if (result == NULL) exit (1);

    strcpy(result, s1);
    strcat(result, s2);

    return result;
}
