//主程序
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

/*
 编译：
 gcc test.c -ldl -rdynamic
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

	if((handle = dlopen(myso, RTLD_NOW)) == NULL) {
		printf("dlopen - %sn", dlerror());
		exit(-1);
	}

	return 0;
}
