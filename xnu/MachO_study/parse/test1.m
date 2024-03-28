#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "parse.h"

int main(int argc, char *argv[])
{
    int fd;
    struct stat st;
    unsigned char *mem;
    
    if (argc < 2)
    {
        printf("Usage: %s <executable>\n", argv[0]);
        exit(0);
    }
    
    if ((fd = open(argv[1], O_RDONLY)) < 0)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }
    
    if (fstat(fd, &st) < 0)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }
    
    mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mem == MAP_FAILED)
    {
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }
    // printf("file size:0x%llx\n",st.st_size);

    parse((void *)mem);

    // close(fd);

    return 0;
}
