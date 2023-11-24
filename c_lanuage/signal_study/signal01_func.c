#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/*
 header file:
 #include <signal.h>

 function:
 void (*signal(int sig, void (*func)(int)))(int);

 Description:
 设置一个函数来处理信号,即带有 sig 参数的信号处理程序

参数介绍
param1: sig	在信号处理程序中作为变量使用的信号码
   SIGABRT	   (Signal Abort) 程序异常终止
   SIGFPE	   (Signal Floating-Point Exception) 算术运算出错,如除数为 0 或溢出（不一定是浮点运算）
   SIGILL	   (Signal Illegal Instruction) 非法函数映象,如非法指令,通常是由于代码中的某个变体或者尝试执行数据导致的
   SIGINT	   (Signal Interrupt) 中断信号,如 ctrl-C,通常由用户生成
   SIGSEGV	   (Signal Segmentation Violation) 非法访问存储器,如访问不存在的内存单元
   SIGTERM	   (Signal Terminate) 发送给本程序的终止请求信号

param2: func 一个指向函数的指针它可以是一个由程序定义的函数,也可以是下面预定义函数之一:
   SIG_DFL	   默认的信号处理程序
   SIG_IGN	   忽视信号

  return value:
  该函数返回信号处理程序之前的值,当发生错误时返回 SIG_ERR
 **/

void sighandler(int signum);

int main()
{
   signal(SIGINT, sighandler);

   while (1)
   {
      printf("开始休眠一秒钟...\n");
      sleep(1);
   }

   return 0;
}

void sighandler(int signum)
{
   printf("捕获信号 %d,跳出...\n", signum);
   exit(1);
}
