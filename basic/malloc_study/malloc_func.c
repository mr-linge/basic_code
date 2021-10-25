#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
header file: #include <stdlib.h>

function:
void *malloc(size_t size)

Description:
分配所需的内存空间，并返回一个指向它的指针

params:
size   	 内存块的大小，以字节为单位。

return value:
该函数返回一个指针 ，指向已分配大小的内存。如果请求失败，则返回 NULL。
*/
int main()
{
   char *str;
 
   /* 最初的内存分配 */
   str = (char *) malloc(16 * sizeof(char));
   strcpy(str, "runoob");
   printf("String = %s,  Address = %p\n", str, str);
 
   /* 重新分配内存 */
   str = (char *) realloc(str, 25);
   strcat(str, ".com");
   printf("String = %s,  Address = %p\n", str, str);
 
   free(str);
 
   return(0);
}
