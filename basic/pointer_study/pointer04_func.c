#include <stdio.h>
/*
 1. void * 指针可以指向任意类型的数据
 
 2. 不允许对 void 指针进行一些算术运算如 p++ 或 p+=1 等，因为既然 void 是无类型，那么每次算术运算我们就不知道该操作几个字节，例如 char 型操作 sizeof(char) 字节，而 int 则要操作 sizeof(int) 字节。
 
 3. 不确定传入或返回值类型的指针时常用 void *
    如:
    void * malloc(size_t size);
    void * memcpy(void *dest, const void *src, size_t len);
    void * memset(void * buffer, int c, size_t num);
 * */
int main() {
	short num = 999;
	
	short *a = &num;
	void *pi;
	// 可以用任意类型的指针对 void 指针赋值,无需进行强制类型转换
	pi = a;
	// 将 void 指针 p 赋给其他类型的指针，则需要强制类型转换
	short *b = (short *) pi;
	printf("num = %d\n", *b);

	return 0;
}
