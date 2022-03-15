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

enum CryptoType {encrypt,decrypt};
enum CryptoType ctype;


// 把数据写到指定路径的目录里,如果目录不存在则创里的内容
void write_message(char *path, char *data, unsigned long len) {
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
	//	FILE *fp = NULL;
	//	fp = fopen(path, "w+");
	//	fprintf(fp, "abc");
	//	fclose(fp);
}

void common_func(char *path) {
	puts("common_func 11 ---->");
	char *new_path = (char *)calloc(128,1);
	puts("common_func 12 ---->");
	strcat(new_path,path);
	puts("common_func 13 ---->");
	strcat(new_path,".temp");
	puts("common_func 14 ---->");
	printf("new_path : \n%s\n", new_path);
	char * blocks = "AAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
	write_message(new_path, blocks, strlen(blocks));
	puts("common_func 15 ---->");

	if(access(path, F_OK) == 0) {
		int ret = remove(path);//删除文件
		puts("common_func 16 ---->");
		if(ret < 0) {
			perror("delete directory fail!");
			exit(1);
		}
	}
	puts("common_func 17 ---->");
	free(new_path);
	puts("common_func 18 ---->");
	free(path);
}

// 生成解密目录 decrypt_directory
void decrypt_directory(char *path) {
	common_func(path);
}

// 生成加密目录 encrypt_directory
void encrypt_directory(char *path) {
	common_func(path);
}

void readFileList(char *basePath) {
	DIR *dir;
	struct dirent *ptr;

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
			//printf("d_name:%s/%s\n",basePath,ptr->d_name);
			puts("readFileList 11 ----->");
			char *current_path = (char *)calloc(128,1);
			strcat(current_path,basePath);
			strcat(current_path,"/");
			strcat(current_path,ptr->d_name);
			printf("readFileList current_path:\n %s\n",current_path);

//			pid_t pid = fork();
//			if (pid == 0) {
				if (ctype == encrypt) {
					encrypt_directory(current_path);
				} else if (ctype == decrypt) {
					decrypt_directory(current_path);
				}
//			} else if (pid < 0) {
//				printf("fork fail\n");
//				exit(-1);
//			}
		} else if (ptr->d_type == 4) {    ///dir
			puts("readFileList 20 ----->");
			//	char base[512];
			//	memset(base,'\0',sizeof(base));
			char *base = (char *) calloc(256,1);
			puts("readFileList 21 ----->");
			strcpy(base,basePath);
			strcat(base,"/");
			strcat(base,ptr->d_name);
			readFileList(base);
			puts("readFileList 24 ----->");
			free(base);
		}
	}
	puts("readFileList 25 ----->");
	closedir(dir);
}

int main(int argc,char *argv[]) {
	char path2[] = "/home/dio/Repositories/C_study/my_func/crypto/test";
	char *path = (char *)malloc(128);
	strncpy(path,path2,sizeof(path2));

	int c = 0;
	while((c = getopt(argc,argv,"ed")) != -1){
		switch(c){
			case 'e':
				{
					ctype = encrypt;
					readFileList(path);
				}
				break;
			case 'd':
				{
					ctype = decrypt;
					readFileList(path);
				}
				break;
			default:
				printf("not find in optstring, optarg = %s\n",optarg);
				break;
		}
	}

	return 0;
}
