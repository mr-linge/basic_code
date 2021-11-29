#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*
   C 库函数 char *strncpy(char *dest, const char *src, size_t n) 把 src 所指向的字符串复制到 dest，最多复制 n 个字符。当 src 的长度小于 n 时，dest 的剩余部分将用空字节填充。

   声明
   下面是 strncpy() 函数的声明。

   char *strncpy(char *dest, const char *src, size_t n)
   参数
   dest -- 指向用于存储复制内容的目标数组。
   src -- 要复制的字符串。
   n -- 要从源中复制的字符数。
   返回值
   该函数返回最终复制的字符串。
 * */

// 生成解密目录
void decrypt_directory(char *a2) {
	char new_path[512];// 这样定义数组后接下来最好初始化一下，不然以后使用时可能会有些脏数据
	memset(new_path,'\0',sizeof(new_path));
	strncpy(new_path,a2,strlen(a2)-5);
	printf("func \n%s====\n",new_path);
	printf("func new_path length : %lu\n",sizeof(new_path));
	printf("func new_path length : %lu\n",strlen(new_path));
}

int main(int argc,char *argv[]) {
	char a2[] = "./test/name1.txt.temp";
	decrypt_directory(a2);
	return 0;
}
