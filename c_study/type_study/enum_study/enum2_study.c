#include <stdio.h>

enum CryptoType {encrypt,decrypt};
enum CryptoType ctype;

void func(int type) {
	if (type == encrypt) {
		puts("encrypt");
	}else if (type == decrypt) {
		puts("decrypt");
	}
}

int main() {
	ctype = decrypt;
	func(ctype);
	return 0;
}
