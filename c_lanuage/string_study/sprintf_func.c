#include <stdio.h>
#include <string.h>
#include <errno.h>

/*
Header:       #include <stdio.h>
Function:     int sprintf(char *str, const char *format, [arg1, arg2, ... ]);
Description:  按照 format 生成字符串, copy 到 str 指向的内存中
params:
   str          字符串指针
   format       字符串的格式化模型
   arg1, arg2   所需拼接的参数

return:
   如果成功,返回值为 format 格式化字符串的长度,失败返回-1
 **/

int main()
{
  int retVal, arg1;
  char *arg2 = NULL;
  char text[0x100] = {0};

  arg1 = 0x10;
  arg2 = "Success";
  retVal = sprintf(text, "This is arg1:%d, arg2:%s,over.", arg1, arg2);
  if (retVal == -1)
  {
    fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
    return -1;
  }
  printf("retVal = %d\n", retVal);
  printf("text:%s\nlength = %lu\n", text, strlen(text));

  return 0;
}
