#include <stdio.h>
#include "vaddr_by_symbol.h"

char *lib_path = "./libc.so";
char *lib_path2 = "./libinject.so";
char *lib_path3 = "./libtest.so";

void search_offset_by_symbol() {
	char *symbol = "printf_vaddr";
	int bind = STB_GLOBAL;
	int type = STT_OBJECT;
	unsigned long offset = offset_symbol(symbol,lib_path3,bind,type);
	printf("%s offset =  0x%lx\n",symbol,offset);	
}

void search_func2() {
	char *symbol = "printf_vaddr";
	int bind = STB_GLOBAL;
	int type = STT_OBJECT;
	unsigned long offset = offset_symbol(symbol,lib_path2,bind,type);
	printf("%s offset =  0x%lx\n",symbol,offset);	
}

void search_func() {
	char *symbol = "printf";
	int bind = STB_GLOBAL;
	int type = STT_FUNC;
	unsigned long offset = offset_symbol(symbol,lib_path,bind,type);
	printf("%s offset =  0x%lx\n",symbol,offset);	
}

void search_var() {
	char *symbol = "printf_vaddr";
	int bind = STB_GLOBAL;
	int type = STT_OBJECT;
	unsigned long offset = offset_symbol(symbol,lib_path,bind,type);                        
	printf("%s offset =  0x%lx\n",symbol,offset);
}

int main() {
	//search_func();
	//puts("********************************");
	//	search_var();
	// search_func2();
	search_offset_by_symbol();

	return 0;
}
