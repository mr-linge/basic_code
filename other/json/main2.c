#include <stdio.h>
#include <string.h>
#include "cJSON.h"

int main()
{
	//1. 创建cJSON对象
	cJSON* root = cJSON_CreateObject();

	//2. 创建对象数据1
	cJSON* item1 = cJSON_CreateObject();
	cJSON_AddStringToObject(item1, "text","我是一个字符串数据1");
	cJSON_AddNumberToObject(item1,"number",666);
	cJSON_AddBoolToObject(item1, "state1", cJSON_False);
	cJSON_AddBoolToObject(item1, "state2", cJSON_True);
	cJSON_AddNullToObject(item1, "state3");
	cJSON_AddItemToObject(root, "data1", item1);

	//3. 创建对象数据2
	cJSON* item2 = cJSON_CreateObject();
	cJSON_AddStringToObject(item2, "text", "我是一个字符串数据2");
	cJSON_AddNumberToObject(item2, "number", 666);
	cJSON_AddBoolToObject(item2, "state1", cJSON_False);
	cJSON_AddBoolToObject(item2, "state2", cJSON_True);
	cJSON_AddNullToObject(item2, "state3");
	cJSON_AddItemToObject(root, "data2", item2);

	//3. 打印生成的结果
	char *json_data=cJSON_PrintUnformatted(root);
	printf("%s\n",json_data);

	//4. 释放空间
	cJSON_Delete(root);
	return 0;
}
