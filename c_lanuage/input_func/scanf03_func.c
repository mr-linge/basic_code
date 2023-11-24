#include <stdio.h>
#include <unistd.h>

int main()
{
	int status = 0;
	while (1)
	{
		printf("****************** pid:%d *******************\n", getpid());
		printf("input:");
		scanf("%d", &status);
		printf("status:%d\n", status);
		switch (status)
		{
		case 1:
		{
			puts("status = 1");
			break;
		}
		case 20:
		{
			puts("status = 20");
			break;
		}
		case 100:
		{
			puts("status = 100");
			break;
		}
		case 999:
			goto done;
		default:
			// puts("输入命令不合法");
			perror("输入命令不合法");
			break;
		}
		sleep(3);
	}

done:
	puts("program is over!");

	return 0;
}
