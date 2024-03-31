#include <stdio.h>
#include <string.h>

/*
Header: 		#include <string.h>
Function:		void *memcpy(void *str1, const void *str2, size_t n)
Description: 	将 str2 起始地址 后 n 个 数据，复制到 sr1 所指向的区域

params:
str1		指向用于存储复制内容的目标数组，类型强制转换为 void* 指针
str2		指向要复制的数据源，类型强制转换为 void* 指针
n			要被复制的字节数

return:
   该函数返回一个指向目标存储区 str1 的指针
**/

int main()
{
	char src[50] = "Hello world! To be  linux expert.";

	char d[50];
	// 将字符串复制到数组 d1 中
	memcpy(d, src, strlen(src));
	printf("d = %s\n", d);

	char d1[20];
	// 从第 11 个字符(r)开始复制，连续复制 6 个字符(runoob)
	// 或者 memcpy(d, s+11*sizeof(char), 6*sizeof(char));
	memcpy(d1, src + 11, 6);
	d1[6] = '\0';
	printf("d1 = %s\n", d1);

	char d2[] = "abcdefg";
	printf("使用 memcpy 前: %s\n", d2);
	memcpy(d2, src, strlen(src));
	printf("使用 memcpy 后: %s\n", d2);

	return (0);
}
