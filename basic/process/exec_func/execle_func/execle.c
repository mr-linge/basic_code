#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	printf("file:%s, line:%d, pid:%d\n", __FILE__, __LINE__, getpid());
	char * const envp[] = {"AA=11", "BB=22", NULL};
	/*
	 * param1 param2 传递给 process image(./bin/target) 里 main 函数的 argv[]
	 * ./bin/target 里的 extern char** environ; 保存的就是自己定义的环境变量char * const envp[]
	 */
	// 利用execle函數传递自己定义的环境变量信息 envp：
	int ret = execle("../target.out", "param 1","param 2", NULL, envp);
	if(ret == -1) {
		perror("execl error");
	}
	printf("Exiting main ...\n");
	return 0;
}
