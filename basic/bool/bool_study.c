#include <stdio.h>

/*
   0 is false
   not 0 is ture
 * */
void main() {
	int b3 = -1;
	int b4 = 0;
	int b5 = 1;
	if(b3) {
		printf("b3 is true, -1 is true\n");
	}
	if(!b4) {
		printf("b4 is false, 0 is false\n");
	}
	if(b5) {
		printf("b5 is true, 1 is ture\n");
	}
}
