#include <stdio.h>
#include <unistd.h>

int global_var = 10;

int func1(int p1, int p2) {
	int tmp = global_var * 10;
	return tmp + p1 + p2;
}

int main() {
	while(1) {
		global_var += 1;
		int ret = func1(1,2);
		printf("result = %d\n", ret);
		sleep(10);
	}
	return 0;
}
