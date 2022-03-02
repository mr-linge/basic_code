#include <stdio.h>
#include "./file1.h"
#include "./file2.h"

int main() {
  int a = 8;
  int b = 9;
  int res1 = foo(a,b);
  int res2 = foo2(a,b);
  printf("res1 = %d, res2 = %d\n",res1, res2);
 return 0;
}
