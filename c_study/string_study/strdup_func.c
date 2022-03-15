#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
#include <string.h>

char * strdup(const char *s)

description:
strdup()会先用maolloc()配置与参数s 字符串相同的空间大小，然后将参数s 字符串的内容复制到该内存地址，然后把该地址返回。该地址最后可以利用free()来释放。
 **/
int main(){
	char a[] = "strdup";
	char *str = strdup(a);
	printf("str: %s\n", str);
	free(str);

	return 0;
}
