#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>


unsigned long splitBlock(uint8_t * message,unsigned long len,uint8_t ** blocks){
    //unsigned long len = strlen(message);
    unsigned long block_num = (len / 16);
    puts("1----->");
    unsigned long mod = len % 16;
    puts("2----->");
    if(mod != 0){
        block_num++;
    }
    puts("3----->");
    *blocks = (uint8_t *)malloc(block_num*16);
    puts("4----->");
    memcpy(*blocks, message, len);
    puts("5----->");
    memset(*blocks+len,0,16-mod);
    puts("6----->");
    return block_num;
}


void main() {
	char * s = "aaaaaaaaaaaaaaaaaaaaaaaaa";
	uint8_t * s2 = (uint8_t *) s;
	uint8_t * blocks = NULL;
	unsigned long ret = splitBlock(s2, 2000, &blocks);
	printf("ret = %ld\n", ret);
}
