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

void readFileList(char *basePath) {
	DIR *dir;
	struct dirent *ptr;
	puts("readFileList 20 ----->");
	//char base[512];
	//memset(base,'\0',sizeof(base));
	char *base = (char *) calloc(512,1);
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
			//printf("d_name:%s/%s\n",basePath,ptr->d_name);
			puts("readFileList 11 ----->");
			char *current_path = (char *)calloc(512,1);
			strcat(current_path,basePath);
			strcat(current_path,"/");
			strcat(current_path,ptr->d_name);
			printf("readFileList current_path:\n %s\n",current_path);
		} else if (ptr->d_type == 4) {    ///dir
			puts("readFileList 23 ----->");
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

void main() {
	char path2[] = "/home/dio/Repositories/C_study/my_func/crypto/test";
	char *path = (char *)malloc(256);
	strncpy(path,path2,sizeof(path2));
	readFileList(path);
}
