#include <stdio.h>
#include <string.h>
#include <malloc.h>

/**********
  在为结构体分配内存时，运行时系统不会自动为结构体内部的指针分配内存。类似地，当结构体消失时，运行时系统也不会自动释放结构体内部的指针指向的内存。
 * ***********/

typedef struct
{
	char *name;
	int score;
}Stu;

void main() {
	Stu * s = (Stu *)malloc(sizeof(Stu));
	s->name = (char *)malloc(16);
	strcpy(s->name,"hello worrld!");
	s->score = 99;
	printf("name : %s\nscore : %d\n",(*s).name,(*s).score);

	free(s->name);   	 //释放内层结构体成员变量
	free(s);		 //释放结构体变量
}
