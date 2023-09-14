#include <stdio.h>

/*
   aligned
   该属性设定一个指定大小的对齐格式(以字节 为单位)
 * */
struct S1
{
	short b[3];
};

struct S2
{
	short b[3];
} __attribute__((aligned(8)));

struct S3
{
	short b[3];
} __attribute__((aligned(16)));

struct S4
{
	short b[3];
} __attribute__((aligned(32)));

struct S5
{
	short b[3];
} __attribute__((aligned(64)));

struct S6
{
	short b[3];
} __attribute__((aligned)); // 使用默认对齐方式，依据目标机器，使用最大最有益的对齐方式

int main(int argc, char **argv)
{
	printf("sizeof(struct S1) = %ld\n", sizeof(struct S1));
	printf("sizeof(struct S2) = %ld\n", sizeof(struct S2));
	printf("sizeof(struct S3) = %ld\n", sizeof(struct S3));
	printf("sizeof(struct S4) = %ld\n", sizeof(struct S4));
	printf("sizeof(struct S5) = %ld\n", sizeof(struct S5));
	printf("sizeof(struct S6) = %ld\n", sizeof(struct S6));

	return 0;
}
