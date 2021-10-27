#include <stdio.h>
#include <string.h>

union Data
{
	int i;
	float f;
	char  str[20];
};

int main( )
{
	union Data data;        
	// 共用体占用的内存应足够存储共用体中最大的成员。
	printf( "Memory size occupied by data : %lu\n", sizeof(data));

	return 0;
}
