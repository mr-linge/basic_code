#include <stdio.h>

/*
   什么是内存对齐？
   不同类型的数据在内存中按照一定的规则排列；而不是顺序的一个接一个的排放，这就是对齐。

   为什么需要内存对齐？
   cpu一次能读取多少内存要看数据总线是多少位，如果是16位，则一次只能读取2个字节，如果是32位，则可以读取4个字节，并且cpu不能跨内存区间访问。
   CPU对内存的读取不是连续的，而是分块读取的，块的大小只能是1、2、4、8、16字节
   当读取操作的数据未对齐，则需要两次总线周期来访问内存，因此性能会大打折扣
   某些硬件平台只能从规定的地址处取某些特定类型的数据，否则则抛出硬件异常

   内存对齐主要遵循下面原则:
   公式1:前面的地址必须是后面的地址整数倍,不是就补齐
   公式2:整个Struct的地址必须是最大字节的整数倍
 **/
struct Test1
{
	char c1;
	short s;
	char c2;
	int i;
};

struct Test2
{
	char c1;
	char c2;
	short s;
	int i;
};

int main(int argc, char **argv)
{
	printf("sizeof(int) = %lu\n", sizeof(int));
	printf("sizeof(struct Test1) = %lu\n", sizeof(struct Test1)); // 12
	printf("sizeof(struct Test2) = %lu\n", sizeof(struct Test2)); // 8

	return 0;
}
