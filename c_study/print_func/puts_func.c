#include <stdio.h>
#include <string.h>
/*
 #include <stdio.h>

 int puts(const char *str)
 
 description:
 把一个字符串写入到标准输出 stdout，直到空字符，但不包括空字符。换行符会被追加到输出中。
 
 params:
 str -- 这是要被写入的 C 字符串。

 retrun value:
 如果成功，该函数返回一个非负值为字符串长度（包括末尾的 \0），如果发生错误则返回 EOF。
 * */
int main() {
   char str1[15];
   char str2[15];

   strcpy(str1, "RUNOOB1");
   strcpy(str2, "RUNOOB2");

   puts(str1);
   puts(str2);
   
   return(0);
}
