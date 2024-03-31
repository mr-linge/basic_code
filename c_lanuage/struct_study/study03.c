#include <stdio.h>

struct Dog
{
    int age;
};

void change_value(struct Dog dog)
{
    printf("%d dog vaddr:%p\n", __LINE__, &dog);
    printf("%d 修改前的形参:%d\n", __LINE__, dog.age);
    // %d 修改实参中的age
    dog.age = 10;
    printf("%d 修改后的形参:%d\n", __LINE__, dog.age);
}

int main()
{
    // 结构体中的数据在 stack 中
    struct Dog dog = {0};
    printf("%d dog vaddr:%p\n", __LINE__, &dog);
    dog.age = 30;
    printf("%d 修改前的实参:%d\n", __LINE__, dog.age);

    change_value(dog);
    printf("%d 修改后的实参:%d\n", __LINE__, dog.age);

    return 0;
}
