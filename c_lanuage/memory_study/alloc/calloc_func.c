#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
   函数原型：
   void * calloc(size_t numElements, size_t sizeOfElement);
   函数功能：
   与malloc相似，参数sizeOfElement为单位元素长度（例如：sizeof(int)），numElements为元素个数，即在内存中申请numElements * sizeOfElement字节大小的连续内存空间。

   注意:
   函数calloc() 会将所分配的内存空间中的每一位都初始化为零
 * */

int main()
{
  char *s = (char *)calloc(64, sizeof(char));
  strcpy(s, "helloworld");
  puts(s);

  return 0;
}
