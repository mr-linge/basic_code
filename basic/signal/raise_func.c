#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/*
 header file: #include <signal.h>

 int raise(int sig)
 Description:
 促使生成信号 sig.

 params:
 sig		信号

 return value:
 如果成功该函数返回零，否则返回非零。 
 * */

void sighandler(int);

int main() {
   signal(SIGINT, sighandler);
  
   int num = 0;
   while(1) {
      printf("开始休眠一秒钟...\n");

      if(num == 5) //5 sec later go here  
      {
	// 生成 SIGINT  信号
      	int ret = raise(SIGINT);
      	if (ret != 0) {
            printf("错误，不能生成SIGINT信号\n");
            exit(-1);
      	}
      }

      sleep(1);
      num++;
   }
  
   return 0; 
}

void sighandler(int signum)
{
   printf("捕获信号 %d，跳出...\n", signum);
   exit(1);
}
