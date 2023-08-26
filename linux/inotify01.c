#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/inotify.h>

int main(int argc, char *argv[])
{

    int fd = inotify_init();
    if (fd < 0)
    {
        fprintf(stderr, "inotify_init failed\n");

        printf("Error no.%d: %s\n", errno, strerror(errno));
        exit(1);
    }

    return 0;
}
