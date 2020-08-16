//
//  study01.c
//  C
//
//  Created by lzd_free on 2019/11/17.
//  Copyright © 2019 Dio. All rights reserved.
//

#include <stdio.h>
#define PI 3.14
#define average(a, b) (a + b) / 2

/*
 在定义函数时，在函数的最左边加上static可以把该函数声明为内部函数(又叫静态函数)，这样该函数就只能在其定义所在的文件中使用。
 如果在不同的文件中有同名的内部函数，则互不干扰。
 **/
static void test(void)
{ //static修饰后 这个函数只能在这个文件内部访问
    printf("调用了test函数\n");
}
void func13(void)
{
    test();
}

void func12(void)
{
    float result = PI * 2;
    printf("result = %f\n", result);
    int max = average(12, 43);
    printf("max = %d\n", max);
}

// 将字符串str中的小写字母变成大写字母，并返回改变后的字符串
// 注意的是：这里的参数要传字符串变量，不能传字符串常量
char *changeUpper(char *str)
{
    // 先保留最初的地址。因为等会str指向的位置会变来变去的。
    char *dest = str;
    // 如果还不是空字符
    while (*str != '\0')
    {
        // 如果是小写字母
        if (*str >= 'a' && *str <= 'z')
        {
            // 变为大写字母。小写和大写字母的ASCII值有个固定的差值
            *str -= 'a' - 'A';
        }

        // 遍历下一个字符
        str++;
    }
    // 返回字符串
    return dest;
}
void func11(void)
{
    // 定义一个字符串变量
    char str[] = "lmj";
    // 调用函数
    char *dest = changeUpper(str);
    printf("%s\n", dest);
}

void func10(void)
{
    // 定义一个指针p
    char *p;
    // 定义一个数组s存放字符串
    char s[] = "mj";
    // 指针p指向字符串的首字符'm'
    p = s; // 或者 p = &s[0];
    for (; *p != '\0'; p++)
    {
        printf("%c \n", *p);
    }
}

void func09(void)
{
    // 定义一个int类型的数组
    int a[4] = {1, 2, 3, 4};
    // 定义一个int类型的指针，并指向数组的第0个元素
    int *p = a;
    int i;
    for (i = 0; i < 4; i++)
    {
        // 利用指针运算符*取出数组元素的值
        int value = *(p + i);

        printf("a[%d] = %d \n", i, value);
    }
}

void func08(void)
{
    char a = 'C';
    printf("修改前，a的值：%c\n", a);
    // 指针变量p指向变量a
    char *p = &a;
    // 通过指针变量p间接修改变量a的值
    *p = 'Z';
    printf("修改后，a的值：%c\n", a);
}

void func07(void)
{
    /*
     scanf函数会从a的首地址开始存放用户输入的字符，存放完毕后，系统会自动在尾部加上一个结束标记\0
     注意，不要写成scanf("%s", &a)，因为a已经代表了数组的地址，没必要再加上&这个地址运算符。
     **/
    //    char a[10];
    //    scanf("%s", a);
    //    printf("the string : %s\n", a);

    /*
     gets跟scanf一样，会从a的首地址开始存放用户输入的字符，存放完毕后，系统会自动在尾部加上一个结束标记\0。
     * gets一次只能读取一个字符串，scanf则可以同时读取多个字符串
     * gets可以读入包含空格、tab的字符串，直到遇到回车为止；scanf不能用来读取空格、tab
     **/
    //    char b[10];
    //    gets(b);
    //    printf("the string2 : %s\n", b);
    // 使用 fgets() 替换 gets()
    char buffer[10];
    fgets(buffer, (sizeof buffer / sizeof buffer[0]), stdin);
    printf("the string2 : %s", buffer);
}

void func06(void)
{
    /*
     其实字符串就是字符序列，由多个字符组成，所以在C语言中，我们可以用字符数组来存储字符串。
     字符串可以看做是一个特殊的字符数组，为了跟普通的字符数组区分开来，应该在字符串的尾部添加了一个结束标志'\0'。
     '\0'是一个ASCII码值为0的字符，是一个空操作符，表示什么也不干。
     所以采用字符数组存放字符串，赋值时应包含结束标志'\0'。
     
     %s表示期望输出一个字符串，因此printf函数会从b的首地址开始按顺序输出字符，一直到\0字符为止，
     因为\0是字符串的结束标记。
     所以，如果想要创建一个字符串，记得加上结束符\0，不然后果很严重，会访问到一些垃圾数据。
     **/

    char a[3] = {'m', 'j', '\0'}; // 添加了结束符\0
    char b[] = {'i', 's'};        // 假设忘记添加结束符\0
    printf("字符串a：%s", a);     // 输出字符串a
    printf("\n");                 // 换行
    printf("字符串b：%s\n", b);   // 输出字符串b
}

void func05(void)
{
    int a[2][3] = {{2, 2, 3}, {3, 4, 5}};
    int tmp = a[0][1];
    printf("a[0][1] = %d\n", tmp);
}

void func04(void)
{
    int a[3] = {2, 3, 4};
    printf("a[0] = %d\n", a[0]);

    int b[3];
    b[0] = 11;
    b[1] = 22;
    b[2] = 33;
    printf("b[0] = %d\n", b[0]);
}

void func03(void)
{
    int c = 11;
    // 以16进制形式输出地址
    printf("16进制:%p\n", &c);
}

void function02(void)
{
    printf("int     is %lu byte\n", sizeof(int));
    printf("double is %lu byte\n", sizeof(double));
}

void function01(void)
{
    int a, b, c;
    printf("请依次输入三个数(用逗号隔开)\n");
    scanf("%d,%d,%d", &a, &b, &c);
    printf("a=%d, b=%d, c=%d\n", a, b, c);
}
