#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int splitBlock(uint8_t * message, unsigned long len, uint8_t ** blocks);

uint8_t * aesEncryptCBC(uint8_t * blocks,uint8_t * key,int block_num,uint8_t * iv);

void aesDecryptCBC(uint8_t * blocks,uint8_t * key,int block_num,uint8_t * iv);

