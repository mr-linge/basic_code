#include <stdio.h>

/*
   argc 当前执行的命令总参数个数
   argv[i] 第i个参数

./params_handle 123 'a' "bbb" "ccccc"
  argc = 5
  argv[0] = ./params_handle
  argv[1] = 123
  argv[2] = a
  argv[3] = bbb
  argv[4] = ccccc
**/
int main(int argc, char *argv[])
{
  printf("argc = %d\n", argc);
  for (int i = 0; i < argc; i++)
  {
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  return 0;
}
