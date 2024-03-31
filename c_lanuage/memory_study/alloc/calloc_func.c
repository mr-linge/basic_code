#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Function: void * calloc(size_t num, size_t size);
Params:
num       单位元素长度
size      元素个数

Description: 在内存中申请 num * size 字节大小的连续内存空间,并将内存空间置0

Return:
   成功,返回一个指针,指向已分配大小的内存
   失败,则返回 NULL
**/

int main()
{
  char *ptr = (char *)calloc(64, sizeof(char));
  strcpy(ptr, "helloworld");
  puts(ptr);

  free(ptr);
  return 0;
}
