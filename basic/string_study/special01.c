#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <stdint.h>

void encrypt_directory(char *path) {
        puts("1-1 ----->");
        unsigned long *len=(unsigned long *)malloc(sizeof(unsigned long));
        //printf("*len = %lu\n", *len);
        uint8_t * blocks = NULL;
        puts("1-2 ----->");
        //char new_path[512];
        char *new_path = (char *)malloc(512);
        memset(new_path,'\0',512);
        puts("10 ----->");
        strncpy(new_path,path,strlen(path)-5);
        puts(new_path);

}

// 生成解密目录
void decrypt_directory(char *path) {
	puts("2-1 ----->");
	unsigned long *len=(unsigned long *)malloc(sizeof(unsigned long));
	//printf("*len = %lu\n", *len);
	uint8_t * blocks = NULL;
	puts("2-2 ----->");
	//char new_path[512];
	char *new_path = (char *)malloc(512);
	memset(new_path,'\0',512);
	puts("20 ----->");
	strncpy(new_path,path,strlen(path)-5);
	puts(new_path);
}

void main() {
	char path2[] = "./test/name.txt.temp";
	char *path = (char *)malloc(256);
	strncpy(path,path2,sizeof(path2));
	for(int i=0; i < 5; i++) {
		decrypt_directory(path);
		encrypt_directory(path);
	}
}       
