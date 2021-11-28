#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

#include "./aes128.h"

// 根据路径获取目录里的内容
char * get_message(char *path) {
	struct stat buf;
	stat(path, &buf);

	//char buffer[buf.st_size];
	char *buffer = (char *) malloc(buf.st_size * sizeof(char));
	int fd = open(path, O_RDONLY);
	int size = read(fd, buffer, buf.st_size);
	if(size < 0){
		perror("open file fail!"); 
	}
	close(fd);
	//    printf("buffer: %s\n", buffer);
	return buffer;
}

// 把数据写到指定路径的目录里,如果目录不存在则创里的内容
void write_message(char *path, uint8_t *data, int len){
	int fd = open(path, O_WRONLY|O_CREAT);
	if(fd < 0) {
		perror("create directory fail!");
	}
	int size = write(fd, data, len);
	if(size < 0){
		perror("open file fail!");
	}
	close(fd);
}

// char message[]="xuanxuanblingbling";
uint8_t key[16]="1234567890123456";
uint8_t iv[16]="1234567890123456";
// 生成加密目录
void encrypt_directory(char *path) {
	char *plaintext = get_message(path);
	uint8_t * blocks = NULL;
	int block_num = splitBlock(plaintext,&blocks);
	aesEncryptCBC(blocks,key,block_num,iv);
	printf("ciphertext:\n");
	for(int i=0;i<block_num*16;i++){
		printf("%02x",blocks[i]);
	}
	printf("\n");
	char new_path[512];
	strcat(new_path,path);
	strcat(new_path,".temp");
	write_message(new_path, blocks, block_num*16);
	int ret = remove(path);//删除文件
        if(ret < 0) {
                perror("delete directory fail!");
	}
	free(blocks);
}
// 生成解密目录
void decrypt_directory(char *path) {
	char *ciphertext = get_message(path);
	uint8_t * blocks = NULL;
	int block_num = splitBlock(ciphertext,&blocks);
	block_num = strlen(ciphertext) / 16;
	aesDecryptCBC(blocks,key,block_num,iv);
	printf("plaintext:\n%s\n",blocks);
	char new_path[512];
	strncpy(new_path,path,strlen(path)-5);
	printf("\n%s====\n",new_path);
	printf("new_path length : %lu\n",sizeof(new_path));
	printf("new_path length : %lu\n",strlen(new_path));

	write_message(new_path, blocks, block_num*16);
	int ret = remove(path);//删除文件
        if(ret < 0) {
                perror("delete directory fail!");
	}
	free(blocks);
}

int main(int argc,char *argv[]) {
	int c = 0;
	while((c = getopt(argc,argv,"ed")) != -1){
		switch(c){
			case 'e':
				{
					char path[] = "./test/name1.txt";
					puts("");
					encrypt_directory(path);
				}
				break;
			case 'd':
				{
					char path[] = "./test/name1.txt.temp";
					puts("");
					decrypt_directory(path);
				}
				break;
			default:
				printf("not find in optstring, optarg = %s\n",optarg);
				break;
		}
	}

	return 0;
}
