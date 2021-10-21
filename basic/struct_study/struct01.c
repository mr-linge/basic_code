#include <stdio.h>
/*
 结构体性质:
 1.定义结构体类型，只是说明了该类型的组成情况，并没有给它分配存储空间，就像系统不为int类型本身分配空间一样。
 只有当定义属于结构体类型的变量时，系统才会分配存储空间给该变量
 2.结构体变量占用的内存空间是其成员所占内存之和，而且各成员在内存中按定义的顺序依次排列
 **/

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

int main() {
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
    return 0;
}
