#include <stdio.h>

int main ()
{
	/* 局部变量定义 */
	char grade = 0;
	puts("intput a char:");
	grade = getchar();
	switch(grade) {
		case 'A' :
			printf("很棒！\n" );
			break;
		case 'B' :
		case 'C' :
			printf("做得好\n" );
			break;
		case 'D' :
			printf("您通过了\n" );
			break;
		case 'F' :
			printf("最好再试一下\n" );
			break;
		default :
			printf("无效的成绩\n" );
			break;
	}

	printf("您的成绩是 %c\n", grade);

	return 0;
}
