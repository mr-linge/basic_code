#include <stdio.h>
#include <string.h>

/*******************
  描述
  C 库函数 char *strstr(const char *haystack, const char *needle) 在字符串 haystack 中查找第一次出现字符串 needle 的位置，不包含终止符 '\0'。

  声明
  下面是 strstr() 函数的声明。

  char *strstr(const char *haystack, const char *needle)
  参数
  haystack -- 要被检索的 C 字符串。
  needle -- 在 haystack 字符串内要搜索的小字符串。
  返回值
  该函数返回在 haystack 中第一次出现 needle 字符串的位置，如果未找到则返回 null。
 *******************/
int main()
{
	const char haystack[20] = "RUNOOB";
	printf("haystack: %s, 		addr : %p\n", haystack,  (void *)haystack);
	const char needle[10] = "NOOB";

	char *ret = strstr(haystack, needle);
	if (ret == NULL) {
		puts("no target string!");
		return -1;
	}

	printf("子字符串是： %s, 	addr : %p\n", ret, ret);

	return(0);
}
