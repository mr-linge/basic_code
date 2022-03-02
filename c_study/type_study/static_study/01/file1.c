#include "./file1.h"
#include <stdio.h>

/*
 在定义函数时，在函数的最左边加上static可以把该函数声明为内部函数(又叫静态函数)，这样该函数就只能在其定义所在的文件中使用。
 如果在不同的文件中有同名的内部函数，则互不干扰。
 **/
static int handle(int a, int b){
  return (a + b) / 2;
}

// h2() 不加 static 会与file2.c 里的 h2() 重名
/*
void h2(){
 printf("file1 h2");
}
*/

int foo(int a, int b){
//  h2();
  return handle(a,b);
}
