#include <stdio.h>
#include <string.h>
#include <errno.h>

/*
header: #include <stdio.h>
函数: int remove(const char *filename)
功能: 删除给定的文件名 filename，以便它不再被访问。

参数
filename -- 这是 C 字符串，包含了要被删除的文件名称。

返回值
如果成功，则返回零。如果错误，则返回 -1，并设置 errno。
**/

const char *path = "/tmp/test.txt";

int main()
{
	int status = remove(path);
	if (status == -1)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	printf("删除文件成功\n");
	return 0;
}
