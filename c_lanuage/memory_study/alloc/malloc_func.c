#include <stdio.h>
#include <stdlib.h>

/*
Header:   #include <stdlib.h>
Function: void *malloc(size_t size)
Params:
   size   	 内存块的大小,以字节为单位
Description: 申请 size 个连续内存空间,但不初始化,也就意味着所分配的内存中可能有其他数据

Return:
   成功,返回一个指针,指向已分配大小的内存
   失败,则返回 NULL
**/

int main()
{
   char *str;

   /* 最初的内存分配 */
   str = (char *)malloc(16 * sizeof(char));
   strcpy(str, "runoob");
   printf("String = %s,  Address = %p\n", str, str);

   free(str);

   return (0);
}
