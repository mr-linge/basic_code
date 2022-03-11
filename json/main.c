#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

int main() {
	FILE *f;//输入文件
	long len;//文件长度
	char *content;//文件内容
	cJSON *json;//封装后的json对象
	f=fopen("./test.json","rb");
	fseek(f,0,SEEK_END);
	len=ftell(f);
	fseek(f,0,SEEK_SET);
	content=(char*)malloc(len+1);
	fread(content,1,len,f);
	fclose(f);

	json = cJSON_Parse(content);
	if (!json) {
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
	}
	char *out = cJSON_Print(json);
	printf("json:\n%s\n",out);
	char *path = cJSON_GetObjectItem(json,"path")->valuestring;
	int number = cJSON_GetObjectItem(json,"number")->valueint;
	int state = cJSON_GetObjectItem(json,"state")->valueint;
	int state2 = cJSON_GetObjectItem(json,"state2")->valueint;
	int state3 = cJSON_GetObjectItem(json,"state3")->valueint;
	//打印
	printf("path:%s\n", path);
	printf("number:%d\n", number);
	printf("state:%d\n", state);
	printf("state2:%d\n", state2);
	printf("state3:%d\n", state3);
	//打印字符串数组
	cJSON *url = cJSON_GetObjectItem(json,"url");
	int array_size = cJSON_GetArraySize(url);
	cJSON *item;
	for(int i=0; i< array_size; i++) {  
		item = cJSON_GetArrayItem(url, i);   
		printf("%s\n",item->valuestring);  
	}
	// 打印字典
	cJSON *sec = cJSON_GetObjectItem(json,"sec");
	char *name = cJSON_GetObjectItem(sec,"name")->valuestring;
	printf("sec{name:%s}\n",name);
	int age = cJSON_GetObjectItem(sec,"age")->valueint;
	printf("sec{age:%d}\n",age);

	// 释放空间
	cJSON_Delete(json);

	return 0;
}
