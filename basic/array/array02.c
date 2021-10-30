#include <stdio.h>

int main() {
	int arr[] = {12,34,34,34,54,36,36,46,32,67,43,65,32,77};
	unsigned long len = (unsigned long) (sizeof(arr) / sizeof(int));
	for(int i = 0; i < len; i++) {
		printf("arr[%d] = %d\t", i, arr[i]);
	}
	printf("\n");
}
