#include <stdio.h>
#include <unistd.h>

/*
指针数组和数组指针的区别

指针数组
指针数组：指针数组可以说成是”指针的数组”，首先这个变量是一个数组。
其次，”指针”修饰这个数组，意思是说这个数组的所有元素都是指针类型。

数组指针
数组指针：数组指针可以说成是”数组的指针”，首先这个变量是一个指针。
其次，”数组”修饰这个指针，意思是说这个指针存放着一个数组的首地址，或者说这个指针指向一个数组的首地址。
根据上面的解释，可以了解到指针数组和数组指针的区别，因为二者根本就是种类型的变量。
**/

// 此处的environ是一個指针数组，它當中的每一個指針指向的char为"XXX=XXX"
extern char **environ;

int main(int argc, char *argv[])
{
    printf("---------------------- environ ----------------------\n");
    for (int i = 0; environ[i] != NULL; i++)
    {
        printf("environ[%d]:%s\n", i, environ[i]);
    }

    printf("---------------------- my define ----------------------\n");
    char * poiner_array[] = {"param1", "param2", NULL};
    for (int i = 0; poiner_array[i] != NULL; i++)
    {
        printf("poiner_array[%d]:%s\n", i, poiner_array[i]);
    }

    return 0;
}
