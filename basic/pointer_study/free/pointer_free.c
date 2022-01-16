#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*********
  char *p1 = (char *) malloc(len);
  char *p2 = p1 + 1;
  只有 alloc 系列函数分配的地址空间 开头保存有这块空间的大小, 可通过 free(p1) 来释放这块空间。
  这片空间上的任何其他位置 都没有这块空间的信息，不能通过free(p2)来释放这块空间
 * *********/
void main() {
	unsigned int len = 32;
	char *p1 = (char *) malloc(len);
	strcpy(p1,"0123456789abcdefghijklmn");
	printf("p1 : %s\n", p1);	

	char *p2 = p1 + 1;
	printf("p2 : %s\n", p2);

	free(p2); // error
}
