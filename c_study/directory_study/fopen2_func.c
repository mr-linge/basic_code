#include <stdio.h>

// fp 会记录当前I/O的位置
int main()
{
    FILE *fp = NULL;
    const unsigned int len = 0x400;
    char buff[len];

    fp = fopen("/tmp/test.txt", "r");

   	fscanf(fp, "%s", buff);
   	printf("1: %s\n", buff);
    
    fgets(buff, len, fp);
    printf("2: %s\n", buff);

    fgets(buff, len, fp);
    printf("3: %s\n", buff);

    fclose(fp);

    return 0;
}
