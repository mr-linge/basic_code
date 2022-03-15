#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void split_block(uint8_t *msg) {
	unsigned long len = strlen(msg);
	unsigned long block_num = len / 16;
	short remainder = (short) (len % 16);
	printf("block_num = %lu\t remainder = %u\n", block_num, remainder);
	if (remainder != 0) {
		block_num++;
		uint8_t *new_p = (uint8_t *) realloc(msg, block_num * 16);
		if (new_p) {
			printf("Memory Reallocated at: %p\n", new_p);
		} else {
			printf("Not Enough Memory!\n");
			exit(-1);
		}
		memset((new_p + (block_num * 16) - (16 - remainder)), '=', (16 - remainder));
		msg = new_p;
		len = block_num * 16;
	}
	printf("len = %lu\n", len);
	printf("new text: %s\n", msg);
}

void main() {
	uint8_t plaintext_text[] = "1234567890abcdef1";
	unsigned long len = (unsigned long) (strlen(plaintext_text) / sizeof(uint8_t));
	printf("len = %lu\n", len);
	uint8_t *text = (uint8_t *) malloc(len);
	strcpy(text,plaintext_text);
	printf("text: %s\n", text);
	split_block(text);
}
