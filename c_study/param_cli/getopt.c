#include <stdio.h>
#include <unistd.h>

/*
   #include <unistd.h>
   int getopt(int argc, char * const argv[], const char * optstring);

   函数说明:
   getopt() 分析命令行参数
   第1,2个参数 argc 和 argv 是由main(int argc, char *argv[])传递的参数个数和内容。
   第3个参数  optstring 则代表欲处理的选项字符串。
   返回值:如果找到符合的参数则返回此参数字母, 如果参数不包含在 optstring 的选项中则返回"?"字符,分析结束则返回-1

optstring
	":" 表示需处理参数,参数写在option后面,不带参数的option可以写在一起
	一个冒号(:)就表示这个选项后面必须带有参数(没有带参数会报错),但是这个参数可以和选项连在一起写,也可以用空格隔开,比如-a123 和 -a 123(中间有空格) 都表示123是-a的参数；
	两个冒号(::)的就表示这个选项的参数是可选的,即可以有参数也可以没有参数,但要注意有参数时,参数与选项之间不能有空格(有空格会报错),这一点和一个冒号时是有区别的。
	相关的全局变量:
	extern char * optarg;	# 保存选项的参数的
	extern int optind;		# 下一个检索位置
	extern int opterr;		# 是否将错误信息输出到stderr，为0时表示不输出
	extern int optopt;		# 不在选项字符串optstring中

例如:
"ab:c:de::" 对应到命令行就是 -a -b"pb" -c"pc" -d -e"pc"
也可以写成 -ad -b"pb" -c"pc" -e"pc"
 * */

/*
./opt -a -b"pb" -c"pc" -d -e
./opt -a -b"pb" -c"pc" -d -e -f
./opt -a -b"pb" -c"pc" -d -e"pc"
*/
int main(int argc, char *argv[])
{
	int ch;
	printf("optind:%d,opterr:%d,optopt:%d\n", optind, opterr, optopt);
	while ((ch = getopt(argc, argv, "ab:c:de::")) != -1)
	{
		printf("optind:%d,opterr:%d,optopt:%d\n", optind, opterr, optopt);
		switch (ch)
		{
		case 'a':
			printf("HAVE option: -a\n\n");
			break;
		case 'b':
			printf("HAVE option: -b\n");
			printf("The argument of -b is %s\n\n", optarg);
			break;
		case 'c':
			printf("HAVE option: -c\n");
			printf("The argument of -c is %s\n\n", optarg);
			break;
		case 'd':
			printf("HAVE option: -d\n");
			break;
		case 'e':
			printf("HAVE option: -e\n");
			printf("The argument of -e is %s\n\n", optarg);
			break;
		case '?':
			printf("Unknown option: %c\n", (char)optopt);
			break;
		}
	}

	return 0;
}
