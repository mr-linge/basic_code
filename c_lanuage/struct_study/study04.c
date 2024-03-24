#include <stdio.h>

int main()
{
    // 定义一个结构体类型
    struct Student
    {
        char *name;
        int age;
    };

    // 定义一个结构体变量
    struct Student stu = {"MJ", 27};

    // 定义一个指向结构体的指针变量
    struct Student *p;

    // 指向结构体变量stu
    p = &stu;

    /*
     这时候可以用3种方式访问结构体的成员
     */
    // 方式1：结构体变量名.成员名
    printf("name=%s, age = %d \n", stu.name, stu.age);
    // 方式2：(*指针变量名).成员名
    printf("name=%s, age = %d \n", (*p).name, (*p).age);
    // 方式3：指针变量名->成员名
    printf("name=%s, age = %d \n", p->name, p->age);
    return 0;
}
