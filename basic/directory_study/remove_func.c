#include <stdio.h>

int main() {
	char s[100];
	int ret;
	scanf("%s",s);//输入文件名。
	ret = remove(s);//删除文件
	if(ret==0)//根据结果给出相应提示
		printf("删除文件成功\n");
	else
		printf("删除文件失败\n");
	return 0;
}
