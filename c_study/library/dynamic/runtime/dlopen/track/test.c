#include <stdio.h>
#include <stdlib.h>

/*
 动态库编译: gcc -shared -fPIC -nostartfiles -o mylib.so mylib.c
 * */

//申明结构体类型
typedef struct __test {
    int i;
    void (*echo_fun)(struct __test *p);
}Test;

//申明注册函数原型
void __register(Test *p);

static void __printf(Test *p) {
    printf("i = %d\n", p->i);
}

//动态库申请一个全局变量空间。这种 ".成员"的赋值方式为c99标准
static Test config = {
    .i = 0,
    .echo_fun = __printf,
};

//加载动态库的自动初始化函数
void _init(void) {
    printf("init\n");
    // __register(&config);//调用主程序的注册函数
}
