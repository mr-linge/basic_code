#include <stdint.h>
#include <string.h>
#include <stdlib.h>

int splitBlock(char * message,uint8_t ** blocks);

uint8_t * aesEncryptCBC(uint8_t * blocks,uint8_t * key,int block_num,uint8_t * iv);

void aesDecryptCBC(uint8_t * blocks,uint8_t * key,int block_num,uint8_t * iv);

