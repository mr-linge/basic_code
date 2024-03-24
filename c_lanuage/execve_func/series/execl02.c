#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	printf("file:%s, line:%d, pid:%d\n", __FILE__, __LINE__, getpid());
	/*
	 * param1 param2 传递给 process image(../target) 里 main 函数的 argv[]
	 * 但是同时默认把原进程的环境变量信息传递给了新进程
	 * ../target 里的 extern char** environ; 保存的就是原进程的环境变量信息
	 */
	int ret = execl("../target", "param1", "param2", NULL);
	if (ret == -1)
	{
		perror("execl error");
	}

	printf("file:%s, line:%d, pid:%d\n", __FILE__, __LINE__, getpid());
	return 0;
}
