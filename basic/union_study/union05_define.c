#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 *  用 union 处理数据
 *  将读取到的 binary data 存到 val 里
 *  然后以 char 的形式读出
 * */
const int long_size = sizeof(long);
union
{
	long val;
	char str[8];
} data;

int main()
{

	// make str into binary data
	//char str[] = {'H','e','l','l','o',',','L','i'};
	//for(int i = (sizeof(str) - 1); i >= 0; i--) {
	//   printf("%x",str[i]);
	//}
	long memory_data = 0x694c2c6f6c6c6548;
	data.val = memory_data;

	char *value = (char *) malloc(long_size + 1);
	memcpy(value,data.str,long_size);
	value[long_size + 1] = '\0';

	printf("data.str : %s\n",value);
	printf("data.str : %s\n",data.str);
	printf("data.val : %lx\n",data.val);
	return 0;
}
