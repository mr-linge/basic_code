#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    unsigned long len = 64;
    unsigned char *mem_space = (unsigned char *)malloc(len);
    memset(mem_space, 'a', 32);
    while (1)
    {
        printf("************ pid:%d mem_space vaddr:%p ************\n", getpid(), mem_space);
        for (size_t i = 0; i < len; i++)
        {
            printf("%02x ", (int)mem_space[i]);
            if ((i + 1) % 0x10 == 0)
            {
                puts("");
            }
        }
        getchar();
    }

    return 0;
}