#include <stdio.h>
#include <string.h>

int main()
{
    // 一个字符串数组 它的下标代表英文月份的阿拉伯数字
    char *months[] = {"January", "February", "March", "April", "May", "June",
                      "July", "August", "September", "October", "November", "December", NULL};
    unsigned long index = 0;
    char *item = NULL;

    while ((item = months[index]) != NULL)
    {
        printf("%s\n", item);
        index++;
    }

    return 0;
}
