#include <stdio.h>

struct Dog {
    int age;
};
void testStruct(struct Dog dog) {
    printf("修改前的形参：%d \n", dog.age);
    // 修改实参中的age
    dog.age = 10;
    printf("修改后的形参：%d \n", dog.age);
}

int main() {
    struct Dog dog = {30};
    printf("修改前的实参：%d \n", dog.age);
    // 调用test函数
    testStruct(dog);
    printf("修改后的实参：%d \n", dog.age);  
    return 0;
}
