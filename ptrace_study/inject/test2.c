#include <stdio.h>
#include <string.h>

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

int main(int argc, char const *argv[])
{
    char *lib_path = "./a.so";
    char arr[0x100] = {0};
    unsigned long len = strlen(lib_path) + 1;
    printf("sizeof(lib_path):%lu\n", len);
    memcpy(arr, (void *)lib_path, len);
    printf("path:%s\n",arr);
    log_matchine_code(arr, 0x40);

    return 0;
}
