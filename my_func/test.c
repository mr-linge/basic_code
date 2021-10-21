#include <stdio.h>
#include "./string_handle.h"

int main() {
    // 定义一个字符串变量
    char str[] = "lmjdfdfdfd";
    // 调用函数
    char *dest = to_upper(str);
    printf("%s\n", dest);
}
