#include <stdio.h>

// 使用typedef关键字为各种数据类型定义一个新名字(别名)
typedef int Integer;
typedef unsigned int UInterger;
typedef float Float;
typedef Integer MyInteger; // 在别名的基础上再起一个别名
typedef char *String;      // typedef也可以给指针起别名

// 定义一个结构体
struct MyPoint
{
    float x;
    float y;
};
// 起别名
typedef struct MyPoint Point;         // 给结构体起别名
typedef struct MyPoint *pointAddress; // 给指向结构体的指针起别名
typedef struct
{
    char *name;
    int age;
} Person; // 定义结构体并同时起别名

// 定义枚举类型
enum SeasonN
{
    springN,
    summerN,
    autumnN,
    winterN
};
// 给枚举类型起别名
typedef enum SeasonN SeasN;

// 定义一个sum函数,计算a跟b的和
int sumN(int a, int b)
{
    int c = a + b;
    printf("%d + %d = %d\n", a, b, c);
    return c;
}
typedef int (*MySum)(int, int); // 使用typedef给指向函数的指针类型起别名

int main()
{
    Integer i = -10;
    UInterger ui = 11;
    Float f = 12.39f;
    MyInteger o = 99;
    printf("%d  %d  %.2f  %d\n", i, ui, f, o);

    // 相当于char *str = "This is a string!";
    String str = "This is a string!";
    printf("str:%s\n", str);

    // 定义结构体变量
    Point p;
    //    struct MyPoint p;
    p.x = 10.0f;
    p.y = 20.0f;
    printf("x:%f, y:%f\n", p.x, p.y);

    struct MyPoint p1 = {1.2, 9.9};
    // 定义指针变量
    pointAddress pAdd = &p1;
    // 利用指针变量访问结构体成员
    printf("x=%f,y=%f\n", pAdd->x, pAdd->y);

    Person per = {"JOJO", 20};
    per.age = 18;
    printf("Person name = %s, age = %d\n", per.name, per.age);

    // 定义枚举变量
    SeasN s = springN;
    // 等价于下面这句
    //    enum SeasonN s = springN;
    printf("springN:%d\n", s);

    // 定义一个指向sum函数的指针变量p
    MySum pSum = sumN;
    // 利用指针变量p调用sum函数
    (*pSum)(4, 5);
    return 0;
}
