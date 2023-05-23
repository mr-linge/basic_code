#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *strIn, char *strOut); // 去除字符串首位空格

void getValue(char *keyAndValue, char *key, char *value);
// 根据key得到value

int writeCFG(const char *filename /*in*/, const char *key /*in*/, const char *value /*in*/);
// 写入配置文件

void readCFG(const char *filename /*in*/, const char *key /*in*/, const char **value /*out*/);
// 读取配置文件