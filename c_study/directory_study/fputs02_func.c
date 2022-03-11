#include <stdio.h>

/**********
 * 将字符串放入标准输入/输出流中,就可打印出这些这些字符串
 * ***********/
int main ()
{
	char buff[] = "Hello world! This is a test.\n";
	fputs(buff,stdout); 
	return(0);
}
