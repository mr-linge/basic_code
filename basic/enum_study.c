#include <stdio.h>

//1.先定义枚举类型，再定义枚举变量
enum Season {spring, summer, autumn, winter};
enum Season s;
//2.定义枚举类型的同时定义枚举变量
enum Season2 {spring2, summer2, autumn2, winter2} s2;
//3.省略枚举名称，直接定义枚举变量
enum {spring3, summer3, autumn3, winter3} s3;

int main() {
//    enum Season {spring, summer, autumn, winter} s;
    // 遍历枚举元素
    for (s = spring; s <= winter; s++) {
        printf("枚举元素：%d \n", s);
    }
    return 0;
}
