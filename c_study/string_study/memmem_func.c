#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
    Header:
    #include <string.h>
    
    Function:
    void *memmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);

    Description:
    在一块内存中寻找匹配另一块内存的内容的第一个位置

    param:
    haystack        待查找内存起始地址
    haystacklen     待查找内存大小
    needle          需匹配内存起始地址
    needlelen       需匹配内存大小

    return value:
    返回该位置的指针，如找不到，返回空指针
**/
int main()
{
    unsigned long buflen = 52;
    uint8_t *buf = (uint8_t *)malloc(buflen);
    char *tmp_str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrst";
    memcpy(buf, tmp_str, strlen(tmp_str));
    printf("%p:%s\n", buf, buf);

    char *needle = "CDEFG";
    size_t needlelen = strlen(needle);
    printf("needlelen:%lu\n", needlelen);

    uint8_t *start_vaddr = memmem(buf, buflen, needle, needlelen);
    printf("find_vaddr:%p\n", start_vaddr);

    return 0;
}