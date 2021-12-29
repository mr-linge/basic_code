#include <stdio.h>

/*
  __attribute__ ((packed)) 的作用就是告诉编译器取消结构在编译过程中的优化对齐,按照实际占用字节数进行对齐，是GCC特有的语法。这个功能是跟操作系统没关系，跟编译器有关，gcc编译器不是紧凑模式的
 * */
struct unpacked_struct {
	char c;
	int i;
};

struct packed_struct {
	char c;
	int i;
	struct unpacked_struct s;
}__attribute__ ((__packed__)); // 不对齐，结构体的长度，就是各个变量长度的和

void main() {
	unsigned long size1 = sizeof(struct unpacked_struct);
	printf("struct unpacked_struct size = %lu\n", size1);
	unsigned long size2 = sizeof(struct packed_struct);
	printf("struct packed_struct size = %lu\n", size2);
}
