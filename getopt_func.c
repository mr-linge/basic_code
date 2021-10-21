#include <stdio.h>
#include <unistd.h>

/*
头文件 #include <unistd.h>

定义函数：int getopt(int argc, char * const argv[], const char * optstring);

函数说明：getopt()用来分析命令行参数。
1、参数argc 和argv 是由main()传递的参数个数和内容。
2、参数optstring 则代表欲处理的选项字符串。

此函数会返回在argv 中下一个的选项字母，此字母会对应参数optstring 中的字母。

如果选项字符串里的字母后接着冒号":"，则表示还有相关的参数，全域变量optarg 即会指向此额外参数。
如果getopt()找不到符合的参数则会印出错信息，并将全域变量optarg 设为"?"字符, 如果不希望getopt()印出错信息，则只要将全域变量optarg 设为0 即可。

返回值：如果找到符合的参数则返回此参数字母, 如果参数不包含在参数optstring 的选项字母则返回"?"字符,分析结束则返回-1.
 * */
int main(int argc,char ** argv){
/*
  for(int i = 0;i < argc;i++){
      printf("argv[%d] = %s\n",i,argv[i]);
  }
*/    	
  int c = 0;
  while((c = getopt(argc,argv,"abc:")) != -1){
   switch(c){
	case 'a':
		printf("option is -a, optarg = %s\n",optarg);
		break;
	case 'b':
		printf("option is -b, optarg = %s\n",optarg);
                break;
        case 'c':
                printf("option is -c, optarg = %s\n",optarg);
                break;
	default:
		printf("not find in optstring, optarg = %s\n",optarg);
                break;
   }
  }
/*
  for(int i = 0;i < argc;i++){
      printf("argv[%d] = %s\n",i,argv[i]);
  }
*/
  return 0;
}
