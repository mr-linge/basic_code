#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("current pid: %d\n", getpid());
    setuid(0);
    printf("current pid: %d\n", getpid());
    system("/bin/bash -p");
    printf("current pid: %d\n", getpid());
    while (1)
    {
        printf("current pid: %d\n", getpid());
        sleep(5);
    };

    return 0;
}
