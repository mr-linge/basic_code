#include <stdio.h>

/*
   char* p
   p的含义是指向常量字符的指针 (常量指针)
   char* p="hello";等价于const char* p="hello"
   常量字符：说明hello这个字符串是无法改变的，则不能更改指针指向的内容，来改变字符串hello。
   *(p+2)='w';错误
   指向的指针：是可以改变自身的值，输出p的时候就不是hello了而是别的字符串
   p="newstring";正确
  
   char p[]
   char p[] = "hello";等价于char const* p = "hello"；
   p的含义是指向字符串的指针常量 (指针常量)
   p是数组的引用，引用本身就是指针常量
   指针常量定义：指针常量不能改变本身的值，但是可以改变指向的值
   p="newstring";是不合法
   *(p+2)='w';合法
 * */
int main() {
	//	char *p = "hello";
	//	*(p+2) = 'w';
	//	printf("%s\n",p);

	char p[] = "hello";
	*(p+2) = 'w';
	printf("%s\n",p);

	return 0;
}
