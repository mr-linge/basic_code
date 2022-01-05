#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

/*
   主程序通过dlopen()加载一个.so的动态库文件, 然后动态库会自动运行 _init() 初始化函数, 初始化函数打印一个提示信息, 然后调用主程序的注册函数给结构体重新赋值, 然后调用结构体的函数指针, 打印该结构体的值. 这样就充分的达到了主程序和动态库的函数相互调用和指针的相互传递.
   gcc参数 -rdynamic 用来通知链接器将所有符号添加到动态符号表中（目的是能够通过使用 dlopen 来实现向后跟踪）.

   编译：
   gcc -rdynamic main.c -o main -ldl
 * */

//申明结构体
typedef struct __test {
	int i;
	void (* echo_fun)(struct __test *p);
}Test;

//供动态库使用的注册函数
void __register(Test *p) {
	p->i = 1;
	p->echo_fun(p);
}

int main() {
	void *handle = NULL;
	char *myso = "./mylib.so";

	if ((handle = dlopen(myso, RTLD_NOW)) == NULL) {
		printf("dlopen - %sn", dlerror());
		exit(-1);
	}

	return 0;
}
