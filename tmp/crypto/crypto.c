#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

#include "./aes128.h"

enum CryptoType {encrypt,decrypt};
enum CryptoType ctype;

// 根据路径获取目录里的内容
uint8_t * get_message(char *path, unsigned long * len) {
	struct stat buf;
	stat(path, &buf);
	*len = buf.st_size;
	//char *buffer = (char *) malloc(buf.st_size);
	uint8_t *buffer = (uint8_t *) calloc(*len, sizeof(uint8_t));
	//char *buffer = (char *) malloc(buf.st_size + 1);
	int fd = open(path, O_RDONLY);
	int size = read(fd, buffer, *len);
	if(size < 0){
		perror("open file fail!");
		exit(1);	
	}
	//*(buffer + buf.st_size) = '\0';
	close(fd);
	//    printf("buffer: %s\n", buffer);
	return buffer;
}

// 把数据写到指定路径的目录里,如果目录不存在则创里的内容
void write_message(char *path, uint8_t *data, unsigned long len){
	int fd = open(path, O_WRONLY|O_CREAT);
	if(fd < 0) {
		perror("create directory fail!");
		exit(-1);
	}
	int size = write(fd, data, len);
	if(size < 0){
		perror("open file fail!");
		exit(1);
	}
	close(fd);
}

// char message[]="xuanxuanblingbling";
uint8_t key[16]="1234567890123456";
uint8_t iv[16]="1234567890123456";
// 生成加密目录
void encrypt_directory(char *path) {
	unsigned long *len = (unsigned long *)malloc(sizeof(unsigned long));
	char *new_path = (char *)calloc(512,1);
	uint8_t *plaintext = get_message(path,len);
	//printf("*len = %lu\n", *len);
	uint8_t * blocks = NULL;
	int block_num = splitBlock(plaintext,*len,&blocks);
	aesEncryptCBC(blocks,key,block_num,iv);
	printf("ciphertext:\n");
	for(int i=0;i<block_num*16;i++){
		printf("%02x",blocks[i]);
	}
	printf("\n");
	//	char new_path[512];
	//	char *new_path = (char *)malloc(512);
	//memset(new_path,'\0',512);
	strcat(new_path,path);
	strcat(new_path,".temp");
	write_message(new_path, blocks, block_num*16);
	if(access(path, F_OK) == 0) {
		int ret = remove(path);//删除文件
		puts("common_func 16 ---->");
		if(ret < 0) {
			perror("delete directory fail!");
			exit(1);
		}
	}
	free(new_path);
	free(plaintext);
	free(blocks);
	free(len);
	free(path);
}
// 生成解密目录
void decrypt_directory(char *path) {
	puts("decrypt_directory 15 --->");
	unsigned long *len = (unsigned long *)malloc(sizeof(unsigned long));
	char *new_path = (char *)calloc(512,1);
	puts("decrypt_directory 16 --->");
	uint8_t *ciphertext = get_message(path,len);
	//printf("*len = %lu\n", *len);
	uint8_t * blocks = NULL;
	int block_num = splitBlock(ciphertext,*len,&blocks);
	aesDecryptCBC(blocks,key,block_num,iv);
	printf("plaintext:\n%s\n",blocks);
	strncpy(new_path,path,strlen(path)-5);
	printf("new_path : %s\n", new_path);
	write_message(new_path, blocks, block_num*16);
	if(access(path, F_OK) == 0) {
		int ret = remove(path);//删除文件
		puts("common_func 16 ---->");
		if(ret < 0) {
			perror("delete directory fail!");
			exit(1);
		}
	}
	puts("decrypt_directory 10 --->");
	if (new_path != NULL)
		free(new_path);
	puts("decrypt_directory 11 --->");
	if (ciphertext != NULL)
		free(ciphertext);
	puts("decrypt_directory 12 --->");
	if (blocks != NULL)
		free(blocks);
	puts("decrypt_directory 13 --->");
	if (len != NULL)
		free(len);
	puts("decrypt_directory 14 --->");
	if (path != NULL)
		free(path);
}

void readFileList(char *basePath) {
	DIR *dir;
	struct dirent *ptr;
	puts("readFileList 20 ----->");
	puts("<<<<<<<<<");
//	char base[512];
//	memset(base,'\0',sizeof(base));
	char *base = (char *) calloc(512,1);
	puts(">>>>>>>>>>");
	puts("readFileList 21 ----->");

	if ((dir=opendir(basePath)) == NULL) {
		printf("basePath:%s\n",basePath);
		perror("Open dir error...");
		exit(1);
	}

	while ((ptr=readdir(dir)) != NULL) {
		puts("readFileList 22 ----->");
		if (strcmp(ptr->d_name,".") == 0 || strcmp(ptr->d_name,"..") == 0) {    ///current dir OR parrent dir
			continue;
		} else if (ptr->d_type == 8 || ptr->d_type == 10) {    ///file or link file
			//      printf("d_name:%s/%s\n",basePath,ptr->d_name);
			//char current_path[512];
			//memset(current_path,'\0',sizeof(current_path));
			//	printf("basePath : %s\n", basePath);
			puts("readFileList 11 ----->");
			char *current_path = (char *)calloc(512,1);
			//memset(current_path,'\0',512);
			strcat(current_path,basePath);
			strcat(current_path,"/");
			strcat(current_path,ptr->d_name);
			printf("readFileList current_path:\n %s\n",current_path);
			char *suffix = strrchr(ptr->d_name, '.');
			//printf("suffix : %s\n", suffix);
			//			pid_t pid = fork();
			//			if (pid == 0) {
			if (ctype == encrypt && (strcmp(suffix,".temp") != 0)) {
				encrypt_directory(current_path);
			} else if (ctype == decrypt && (strcmp(suffix,".temp") == 0)) {
				puts("readFileList 12 ----->");
				decrypt_directory(current_path);
			}
			//} else if (pid < 0) {
			//	printf("fork fail\n");
			//	exit(-1);
			//}
			//puts("readFileList 25 ----->");
			//free(current_path);
		} else if (ptr->d_type == 4) {    ///dir
			puts("readFileList 23 ----->");
			strcpy(base,basePath);
			strcat(base,"/");
			strcat(base,ptr->d_name);
			puts("readFileList 24 ----->");
			readFileList(base);
			puts("readFileList 25 ----->");
			free(base);
		}
	}
	puts("readFileList 25 ----->");
	closedir(dir);
}


int main(int argc,char *argv[]) {
	int c = 0;
	while((c = getopt(argc,argv,"ed")) != -1){
		switch(c){
			case 'e':
				{
					char path2[] = "./test";
					char *path = (char *)malloc(256);
					strncpy(path,path2,sizeof(path2));
					ctype = encrypt;
					readFileList(path);
					//	encrypt_directory(path);
				}
				break;
			case 'd':
				{
					char path21[] = "./test";
					char *path1 = (char *)malloc(256);
					strncpy(path1,path21,sizeof(path21));
					ctype = decrypt;
					readFileList(path1);
					//	decrypt_directory(path);
				}
				break;
			default:
				printf("not find in optstring, optarg = %s\n",optarg);
				break;
		}
	}

	return 0;
}
