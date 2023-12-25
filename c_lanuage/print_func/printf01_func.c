#include <stdio.h>

/*
   格式	使用类型						 			用法					说明
   d	用来输出十进制整数				   				%d	   			   按整型数据的实际长度输出。
													%md					m为指定的输出字段的宽度。如果数据的位数小于m，则左端补以空格，若大于m，则按实际位数输出。
													%ld					输出长整型数据。
   u	以无符号十进制形式输出整数						%lu					对长整型可以用"%lu"格式输出
													%mu					同样也可以指定字段宽度用“%mu”格式输出。
   o	以无符号八进制形式输出整数。		 			%lo		 			 对长整型可以用"%lo"格式输出
													%mo		    		指定字段宽度用“%mo”格式输出
   x	以无符号十六进制形式输出整数		 			%lx			 		 对长整型可以用"%lx"格式输出
													%mx					同样也可以指定字段宽度用"%mx"格式输出。
   c	输出一个字符。					   			   %c
   s	用来输出一个串					  			   %s			   	  输出字符串（不包括双引号）。
													%ms					输出的字符串占m列，如字符串本身长度大于m，则突破获m的限制,将字符串全部输出。若串长小于m，则左补空格。
													%-ms				如果串长小于m，则在m列范围内，字符串向左靠，右补空格。
													%m.ns				输出占m列，但只取字符串中左端n个字符。这n个字符输出在m列的右侧，左补空格。
													%-m.ns				其中m、n含义同上，n个字符输出在m列范围的左侧，右补空格。如果n>m，则自动取n值，即保证n个字符正常输出。
   f	用来输出实数（包括单、双精度），以小数形式输出。	%f					不指定宽度，整数部分全部输出并输出6位小数。
													%m.nf				输出共占m列，其中有n位小数，如数值宽度小于m左端补空格。
													%-m.nf				输出共占m列，其中有n位小数，如数值宽度小于m右端补空格。
   e	以指数形式输出实数。							%e				   数字部分（又称尾数）输出6位小数，指数部分占5位或4位。
													%m.ne和%-m.ne		m、n和”-”字符含义与前相同。此处n指数据的数字部分的小数位数，m表示整个输出数据所占的宽度。
   g	自动选f格式或e格式中较短的一种输出，且不输出无意义的零。
**/

int main()
{
	char c1 = 'O';
	unsigned char c2 = 'K';
	printf("char 	   	c1 = %c, size = %lu\n", c1, sizeof(c1));
	printf("unsigned  	c2 = %c, size = %lu\n", c2, sizeof(c2));

	short s1 = 200;
	unsigned short s2 = 200;
	printf("short 	        s1 = %d, size = %lu\n", s1, sizeof(s1));
	printf("unsigned short  s2 = %u, size = %lu\n", s2, sizeof(s2));

	int i1 = 99999;
	unsigned int i2 = 3000000000; /*int为32位*/
	printf("int 	        i1 = %d, size = %lu\n", i1, sizeof(i1));
	printf("unsigned int 	i2 = %u, size = %lu\n", i2, sizeof(i2));

	long l1 = 65537;
	unsigned long l2 = 66666666;
	printf("long 	        l1 = %ld, size = %lu\n", l1, sizeof(l1));
	printf("unsigned long	l2 = %lu, size = %lu\n", l2, sizeof(unsigned long));

	long long ll1 = 12345678908642;
	unsigned long long ll2 = 12345678908642;
	printf("long long	   ll1 = %lld, size = %lu\n", ll1, sizeof(ll1));
	printf("unsigned long long ll2 = %llu, size = %lu\n", ll2, sizeof(unsigned long long));

	float f = 1.001;
	printf("floadt 	         f = %f, size = %lu\n", f, sizeof(float));
	double df = 3852.99;
	printf("double          df = %6.1f, size = %lu\n", df, sizeof(double));

	char buf[] = "CHINA";
	printf("%7.2s", buf);
	printf("\n");

	return 0;
}
