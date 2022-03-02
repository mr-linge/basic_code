#include <stdio.h>

int main() {
    /*
     scanf函数会从a的首地址开始存放用户输入的字符，存放完毕后，系统会自动在尾部加上一个结束标记\0
     注意，不要写成scanf("%s", &a)，因为a已经代表了数组的地址，没必要再加上&这个地址运算符。
     **/
   char a[10];
   scanf("%s", a);
   printf("input string : %s\n", a);
   return 0;
}
