#include <stdio.h>
#include <string.h>

/*
   头文件：#inclue <string.h>

   strncat()用于将n个字符追加到字符串的结尾，其原型为：
   char * strncat(char *dest, const char *src, size_t n);

   [参数说明] dest为目标字符串，src为源字符串，n为要追加的字符的数目。

   strncat()将会从字符串src的开头拷贝n 个字符到dest字符串尾部，dest要有足够的空间来容纳要拷贝的字符串。如果n大于字符串src的长度，那么仅将src全部追加到dest的尾部。

   strncat()会将dest字符串最后的'\0'覆盖掉，字符追加完成后，再追加'\0'。

   [返回值] 返回字符串dest。
**/

int main()
{
  char url[0x100] = "http://see.xidian.edu.cn";
  char path[0x20] = "/cpp/u/biaozhunku/";
  strncat(url, path, 0x10);
  printf("%s\n", url);
  return 0;
}
