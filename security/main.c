#include <stdio.h>
#include <string.h>
#include <stdlib.h>

union Register
{
    unsigned char bytes[8];
    unsigned long val;
};

void should_not_be_called()
{
    printf("%s is running ...\n", __FUNCTION__);
    exit(0);
}

void attack(char *buff)
{
    char *tmp = (char *)malloc(0x100);
    memset(tmp, '\0', 0x100);
    scanf("%s", tmp);
    char *param1 = strsep(&tmp, "|");
    // printf("%d %s\n", __LINE__, param1);
    memcpy(buff, param1, strlen(param1));
    char *param2 = strsep(&tmp, "|");
    if (param2 != NULL)
    {
        // printf("%d %s\n", __LINE__, param2);
        union Register data = {0};
        data.val = strtoul(param2, NULL, 16);
        // printf("address:0x%lX\n", data.val);
        memcpy(buff + strlen(param1), data.bytes, 8);
    }

    for (unsigned long i = 0; i < 0x100; i++)
    {
        printf("%02X ", buff[i]);
        if ((i + 1) % 0x8 == 0)
        {
            printf("\t");
        }
        if ((i + 1) % 0x10 == 0)
        {
            puts("");
        }
    }
    puts("");
    free(tmp);
}

void normal_call()
{
    char buff[0x10] = {0};
    attack(buff);
    printf("%s is over ...\n", __FUNCTION__);
}

int main(int argc, char **argv)
{
    printf("main                    vaddr:%p\n", &main);
    printf("should_not_be_called    vaddr:%p\n", &should_not_be_called);
    printf("normal_call             vaddr:%p\n", &normal_call);

    normal_call();

    return 0;
}
