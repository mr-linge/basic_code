#include <stdio.h>
#include <string.h>
#include "cJSON.h"

int main()
{
	cJSON *width = NULL;
	cJSON *height = NULL;
	int i;
	const unsigned int resolution_numbers[3][2] = {
		{1280, 720},
		{1920, 1080},
		{3840, 2160}
	};

	//1. 创建cJSON对象
	cJSON* root = cJSON_CreateObject();

	//2. 创建数组对象
	cJSON *array = cJSON_CreateArray();
	cJSON_AddItemToObject(root, "text", array);

	for (i = 0; i < (sizeof(resolution_numbers) / (2 * sizeof(int))); ++i)
	{
		cJSON *obj = cJSON_CreateObject();
		cJSON_AddItemToArray(array, obj);

		width = cJSON_CreateNumber(resolution_numbers[i][0]);
		cJSON_AddItemToObject(obj, "width", width);

		height = cJSON_CreateNumber(resolution_numbers[i][1]);
		cJSON_AddItemToObject(obj, "height", height);
	}

	//3. 打印生成的结果
	char *json_data=cJSON_PrintUnformatted(root);
	printf("%s\n",json_data);

	//4. 释放空间
	cJSON_Delete(root);
	return 0;
}
