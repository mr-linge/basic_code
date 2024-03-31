#include <stdio.h>
#include <stdlib.h>

struct MyPoint
{
	float x;
	float y;
};

typedef struct
{
	char *name;
	int age;
} Person; // 定义结构体并同时起别名

int main()
{
	// 结构体中的数据在 heap 中
	struct MyPoint *p = (struct MyPoint *)malloc(sizeof(struct MyPoint));
	p->x = 10;
	p->y = 20;
	printf("x:%f, y:%f\n", p->x, p->y);

	Person *per = (Person *)malloc(sizeof(Person));
	(*per).name = "JOJO";
	(*per).age = 20;
	printf("Person name = %s, age = %d\n", (*per).name, (*per).age);

	return 0;
}
