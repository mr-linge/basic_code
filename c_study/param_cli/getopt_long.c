/*
#include <getopt.h>

int getopt_long(int argc, char * const argv[], const char *optstring, const struct option *longopts, int *longindex);
int getopt_long_only(int argc, char * const argv[],const char *optstring,const struct option *longopts, int *longindex);

getopt_long() 兼容 getopt()
前3个参数与getopt()相同
第4个参数 longopts  是指向option结构的数组，option结构被称为"长选项表"
第5个参数 longindex 参数如果没有设置为 NULL，那么它就指向一个变量，这个变量会被赋值为寻找到的长选项在longopts中的索引值，这可以用于错误诊断。

option结构在getopt.h中的声明如下:
struct option{
    const char *name;
    int has_arg;
    int *flag;
    int val;
};
对结构中的各元素解释如下:
const char *name
这是选项名，前面没有短横线。譬如"help"、"verbose"之类。

int has_arg
描述了选项是否有选项参数。如果有，是哪种类型的参数，此时，它的值一定是下表中的一个。
符号常量 	            数值 	含义
no_argument 	        0	选项没有参数
required_argument	    1	选项需要参数
optional_argument	    2	选项参数可选

int *flag
如果这个指针为NULL，那么 getopt_long()返回该结构val字段中的数值。
如果该指针不为NULL，getopt_long()会使得它所指向的变量中填入val字段中的数值，并且getopt_long()返回0。
如果flag不是NULL，但未发现长选项，那么它所指向的变量的数值不变。

int val
这个值是发现了长选项时的返回值，或者flag不是NULL时载入*flag中的值。典型情况下，若flag不是NULL，那么val是个真／假值，譬如1或0；另一方面，如果flag是NULL，那么 val通常是字符常量，若长选项与短选项一致，那么该字符常量应该与optstring中出现的这个选项的参数相同。
每个长选项在长选项表中都有一个单独条目，该条目里需要填入正确的数值。数组中最后的元素的值应该全是0。数组不需要排序，getopt_long()会进行线性搜索。但是，根据长名字来排序会使程序员读起来更容易。
*/

#include <stdio.h>
#include <getopt.h>

struct option longopts[] = {
    {"test1", no_argument, NULL, 1},
    {"test2", no_argument, NULL, 2},
    {"test3", required_argument, NULL, 3},
    {"test4", required_argument, NULL, 4},
    {0, 0, 0, 0},
};

/*
./optlong --test1 --test2
./optlong --test1 --test2 --test3 "p3" --test4 "p4"
./optlong -a -b"pb" -c"pc" -d -e"pc"
./optlong --test1 --test2 --test3 "p3" --test4 "p4" -a -b"pb" -c"pc" -d -e"pc"
*/
int main(int argc, char *argv[])
{
    int c;
    printf("optind:%d,opterr:%d,optopt:%d\n", optind, opterr, optopt);
    while ((c = getopt_long(argc, argv, "ab:c:de::", longopts, NULL)) != -1)
    {
        printf("optind:%d,opterr:%d,optopt:%d\n", optind, opterr, optopt);
        switch (c)
        {
        case 1:
            printf("--test1 is going\n");
            break;
        case 2:
            printf("--test2 is going\n");
            break;
        case 3:
            printf("The argument of --test3 is %s\n", optarg);
            break;
        case 4:
            printf("The argument of --test4 is %s\n", optarg);
            break;
        case 'a':
            printf("HAVE option: -a\n");
            break;
        case 'b':
            printf("HAVE option: -b\n");
            printf("The argument of -b is %s\n", optarg);
            break;
        case 'c':
            printf("HAVE option: -c\n");
            printf("The argument of -c is %s\n", optarg);
            break;
        case 'd':
            printf("HAVE option: -d\n");
            break;
        case 'e':
            printf("HAVE option: -e\n");
            printf("The argument of -e is %s\n", optarg);
            break;
        case '?':
            printf("Unknown option: %c\n", (char)optopt);
            break;
        }
    }
    return 0;
}