#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
Header:       #include <string.h>
Function:     void bzero(void *s, int n);
Description:  将内存块的前n个字节清零,等价于 memset((void*)s, 0, size_t n)

params:
s为指针,n 为需要清零的字节数

注意:
bzero() 不是标准函数,没有在ANSI中定义,在VC6.0和MinGW5下编译没通过; gcc 和 clang 是支持的
**/

int main()
{
  unsigned long len = 0x100;
  char *s = (char *)malloc(len);

  bzero(s, len);

  char *str = "Hello, will be success!";
  memcpy(s, str, strlen(str));
  printf("s:%s\n", s);

  return 0;
}
