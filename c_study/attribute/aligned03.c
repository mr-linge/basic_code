#include <stdio.h>

/*
   aligned
   该属性设定一个指定大小的对齐格式（以字节 为单位）

   注意：__attribute__属性的效力与你的链接器有关，如果你的链接器最大只支持16字节对齐，那么你此时定义32字节对齐也无济于事。
 * */
struct S1 {
	char c1;
	char c2;
	long l1;
	short b[3];
};

struct S2 {
	char c1;
	char c2;
	long l1;
	short b[3];
} __attribute__ ((aligned (8)));

void main() {
	unsigned long size = sizeof(struct S1);
	printf("struct S1 size = %lu\n", size);	
	unsigned long size2 = sizeof(struct S2);
	printf("struct S2 size = %lu\n", size2);	
}
