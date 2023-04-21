#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

/*
  gcc参数 -rdynamic 用来通知链接器将所有符号添加到动态符号表中（目的是能够通过使用 dlopen 来实现向后跟踪）.
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
	char *myso = "./libtest.so";

	if ((handle = dlopen(myso, RTLD_NOW)) == NULL) {
		printf("dlopen - %sn", dlerror());
		exit(-1);
	}

	return 0;
}
