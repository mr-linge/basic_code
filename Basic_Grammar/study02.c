//
//  study02.c
//  C
//
//  Created by lzd_free on 2019/11/19.
//  Copyright © 2019 Dio. All rights reserved.
//

#include <stdio.h>

//使用typedef关键字为各种数据类型定义一个新名字(别名)
typedef int Integer;
typedef unsigned int UInterger;
typedef float Float;
typedef Integer MyInteger;//在别名的基础上再起一个别名
typedef char* String;//typedef也可以给指针起别名
// 定义一个结构体
struct MyPoint {
    float x;
    float y;
};
// 起别名
typedef struct MyPoint Point;//给结构体起别名
typedef struct MyPoint* pointAddress;//给指向结构体的指针起别名
// 定义枚举类型
enum SeasonN {springN, summerN, autumnN, winterN};
// 给枚举类型起别名
typedef enum SeasonN SeasonN;
// 定义一个sum函数，计算a跟b的和
int sumN(int a, int b) {
    int c = a + b;
    printf("%d + %d = %d\n", a, b, c);
    return c;
}
typedef int (*MySum)(int, int);//使用typedef给指向函数的指针类型起别名
void func19(void) {
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
    printf("x=%f，y=%f\n", pAdd->x, pAdd->y);
    
    // 定义枚举变量
    SeasonN s = springN;
    //等价于下面这句
//    enum SeasonN s = springN;
    printf("springN:%d\n", s);
    
    // 定义一个指向sum函数的指针变量p
    MySum pSum = sumN;
    // 利用指针变量p调用sum函数
    (*pSum)(4, 5);
}

//1.先定义枚举类型，再定义枚举变量
enum Season {spring, summer, autumn, winter};
enum Season s;
//2.定义枚举类型的同时定义枚举变量
enum Season2 {spring2, summer2, autumn2, winter2} s2;
//3.省略枚举名称，直接定义枚举变量
enum {spring3, summer3, autumn3, winter3} s3;
void func18(void) {
//    enum Season {spring, summer, autumn, winter} s;
    // 遍历枚举元素
    for (s = spring; s <= winter; s++) {
        printf("枚举元素：%d \n", s);
    }
}

void func17(void) {
    // 定义一个结构体类型
    struct Student {
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
}

struct Dog {
    int age;
};
void testStruct(struct Dog dog) {
    printf("修改前的形参：%d \n", dog.age);
    // 修改实参中的age
    dog.age = 10;
    printf("修改后的形参：%d \n", dog.age);
}
void func16(void){
    struct Dog dog = {30};
    printf("修改前的实参：%d \n", dog.age);
    // 调用test函数
    testStruct(dog);
    printf("修改后的实参：%d \n", dog.age);
}

//跟结构体变量一样，结构体数组也有3种定义方式
struct Teacher1 {
    char *name;
    int age;
};
struct Teacher1 tea1[5]; //定义1

struct Teacher2 {
    char *name;
    int age;
} tea2[5]; //定义2

struct {
    char *name;
    int age;
} tea3[5]; //定义3

//初始化
struct {
    char *name;
    int age;
} tea4[2] = { {"MJ", 27}, {"JJ", 30} };

void func15(void){
    //也可以用数组下标访问每一个结构体元素，跟普通数组的用法是一样的
    struct Teacher1 teaArr[5] = {{"Dio", 37}, {"JJ", 30},{"MJ", 27}, {"JJ", 30},{"MJ", 27}};
    struct Teacher1 tea = teaArr[0];
    printf("teaArr[0]  name:%s,age:%d\n",tea.name, tea.age);
    
    struct Teacher1 tea21 = {"MMJ", 31};
    struct Teacher1 tea22 = {"MJJ", 30};
    struct Teacher1 tea2Arr[5];
    tea2Arr[0] = tea21;
    tea2Arr[1] = tea22;
    printf("tea2Arr[0] name:%s,age:%d\n",tea2Arr[0].name, tea2Arr[0].age);
}


//1.先定义结构体类型，再定义变量
struct Student {
    char *name;
    int age;
};
struct Student stu;
//2.定义结构体类型的同时定义变量
struct Student2 {
    char *name;
    int age;
} stu2;
//3.直接定义结构体类型变量，省略类型名
struct {
    char *name;
    int age;
} stu3;

//4.结构体内可以包含别的结构体
struct Date {
    int year;
    int month;
    int day;
};
struct Student4 {
    char *name;
    struct Date birthday;
};

/*
 结构体性白质:
 1.定义结构体类型，只是说明了该类型的组成情况，并没有给它分配存储空间，就像系统不为int类型本身分配空间一样。
 只有当定义属于结构体类型的变量时，系统才会分配存储空间给该变量
 2.结构体变量占用的内存空间是其成员所占内存之和，而且各成员在内存中按定义的顺序依次排列
 **/

void func14(void) {
    //结构体的初始化
    //将各成员的初值，按顺序地放在一对大括号{}中，并用逗号分隔，一一对应赋值。
    struct Student stu = {"MJ", 27};
    printf("name:%s\n", stu.name);
    
    // 访问stu的age成员
    stu.age = 29;
    printf("age:%d\n", stu.age);
    
    //如果某个成员也是结构体变量，可以连续使用成员运算符"."访问最低一级成员
    struct Student4 stu4;
    stu4.name = "Dio Brand";
    stu4.birthday.year = 1986;
    stu4.birthday.month = 9;
    stu4.birthday.day = 10;
    printf("name:%s,birthday year:%d,month:%d,day:%d\n",stu4.name,stu4.birthday.year,stu4.birthday.month,stu4.birthday.day);
    
    //相同类型的结构体变量之间可以进行整体赋值 如将stu直接赋值给stu2
    struct Student stu2 = stu;
    printf("name:%s,age:%d\n",stu2.name,stu2.age);
}
