#include <stdio.h>
#include <string.h>
#include "cJSON.h"


//将要解析的JSON数据.
char data[] =
"{"
"\"text\": ["
"{"
"\"width\": 1280,"
"\"height\" : 720"
"},"
"{"
"\"width\": 1920,"
"\"height\" : 1080"
"},"
"{"
"\"width\": 3840,"
"\"height\" : 2160"
"}"
"]"
"}";

int main()
{
	//1. 载入JSON数据
	cJSON* root = cJSON_Parse(data);
	if (root == NULL)return 0;
	//2. 解析字段
	cJSON* item;
	int i;
	item = cJSON_GetObjectItem(root, "text");
	if (item)
	{
		//获取数组的大小
		int ArraySize = cJSON_GetArraySize(item);
		//解析数组的里的每个成员
		for (i = 0; i < ArraySize; i++)
		{
			//取出数组下标对象
			cJSON *array_item = cJSON_GetArrayItem(item, i);
			if (array_item == NULL)continue;

			//解析数据
			cJSON *obj = cJSON_GetObjectItem(array_item, "width");
			if (obj)
			{
				printf("width=%d\n",obj->valueint);
			}
			obj = cJSON_GetObjectItem(array_item, "height");
			if (obj)
			{
				printf("height=%d\n", obj->valueint);
			}
		}
	}

	//3. 释放空间
	cJSON_Delete(root);
	return 0;
}
