#include <stdio.h>
#include <string.h>
#include <stdlib.h>

union Data
{
  unsigned char bytes[8];
  unsigned long val;
};

/*
  char *p = (char *)malloc(0x10);
  malloc 返回的是一个指针p, p保存在 stack栈 中
  p 是所分配内存空间的起始地址,这片内存是在 headp堆 中的
*/
int main()
{
  char *str1 = (char *)malloc(0x10);
  memset(str1, 'A', 0x10);
  printf("str1 vaddr:%p\n", &str1);
  union Data dt1;
  memcpy(dt1.bytes, (void *)&str1, 8);
  printf("dt1.val:0x%lx\n", dt1.val);

  for (int i = 0; i < 0x10; i++)
  {
    printf("%p --> %02X\t", &str1[i], (int)str1[i]);
    if ((i + 1) % 0x4 == 0)
    {
      puts("");
    }
  }
  puts("");

  char *str2 = (char *)malloc(0x10);
  memset(str2, 'B', 0x10);
  printf("str2 vaddr:%p\n", &str2);
  union Data dt2;
  memcpy(dt2.bytes, (void *)&str2, 8);
  printf("dt2.val:0x%lx\n", dt2.val);

  for (int i = 0; i < 0x10; i++)
  {
    printf("%p --> %02X\t", &str2[i], (int)str2[i]);
    if ((i + 1) % 0x4 == 0)
    {
      puts("");
    }
  }
  puts("");

  free(str1);
  free(str2);

  return (0);
}
