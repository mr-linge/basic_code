#include <stdio.h>

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

int main() {
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
  return 0;
}
