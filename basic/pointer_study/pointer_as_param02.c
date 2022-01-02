#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * 在函数外定义一个空指针，传到函数内，在函数内根据需要来操作这个指针。
 * 这时形参需要以二级指针的形式存在。因为形参是将实参复制一份传到函数内，传递一级指针的话，函数外是无法同步函数里面的操作的
 * */
void splitBlock(char **blocks) {
    int len = 16;
    *blocks = (char *) malloc(len);
    memset(*blocks, 'a', len);
}

int main() {
    char *blocks = NULL;
    splitBlock(&blocks);
    printf("blocks = %s\n", blocks);

    return 0;
}
