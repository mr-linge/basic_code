#include <stdio.h>
#include <string.h>

/*
   头文件：#include <string.h>

   定义函数：char * strtok(char *s, const char *delim);

   函数说明：strtok()用来将字符串分割成一个个片段。参数s 指向欲分割的字符串，参数delim 则为分割字符串，当strtok()在参数s 的字符串中发现到参数delim 的分割字符时则会将该字符改为\0 字符。在第一次调用时，strtok()必需给予参数s 字符串，往后的调用则将参数s 设置成NULL。每次调用成功则返回下一个分割后的字符串指针。
   注:第一次调用strtok函数时,这个函数将忽略间距分隔符并返回指向在strToken字符串找到的第一个符号的指针,该符号后以NULL字符结尾.通过调用一系列的strtok函数,更多的符号将从strToken字符串中分离出来.每次调用strtok函数时,都将通过在找到的符号后插入一个NULL字符来修改strToken字符串.为了读取strToken中的下一个符号,调用strtok函数时strToken参数为NULL,这会引发strtok函数在已修改过的strToken字符串查找下一个符号.

   返回值：返回下一个分割后的字符串指针，如果已无从分割则返回NULL。

   在linux2.6.29以后的版本中，strtok被strsep代替了。
 * */
int main()
{
	char s[] = "ab-cd : ef;gh :i-jkl;mnop;qrs-tu: vwx-y;z";
	char *delim = "-: ";
	char *p;
	printf("%s\n", strtok(s, delim));
	puts("\n-------- while before -------");
	while (p = strtok(NULL, delim))
	{
		printf("%s\n", p);
	}
	puts("");

	return 0;
}
