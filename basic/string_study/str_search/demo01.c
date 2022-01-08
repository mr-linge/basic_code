#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

void main() {
	uint8_t * text = (uint8_t *) calloc(16,1);
	strcpy(text, "0123456789");
	printf("text: %s\n", text);
	char *filling = strrchr(text,'\0');
	printf("filling : %s\n", filling);
}
