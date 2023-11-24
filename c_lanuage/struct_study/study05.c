#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 定义一个结构体类型
struct Data
{
    char bytes[0x100];
};

void log_matchine_code(void *vaddr, unsigned long len)
{
    unsigned char *machine_codes = (unsigned char *)vaddr;
    for (unsigned long i = 0; i < len; i++)
    {
        printf("%02x ", (int)machine_codes[i]);
        if ((i + 1) % 4 == 0)
        {
            puts("");
        }
    }
}

int main()
{
    // 结构体数据复制. 此时 dt1,dt2 是栈中的一段内存
    struct Data dt1, dt2 = {0};
    memset((void *)&dt1, 'a', 0x100);
    log_matchine_code(&dt1, 0x10);
    puts("========================================");
    memcpy((void *)&dt2, (void *)&dt1, 0x100);
    log_matchine_code(&dt2, 0x10);

    return 0;
}
