#include <stdio.h>
#include "vaddr_by_symbol.h"


int main(int argc, char **argv) {
        char * inject_library_path = "./libinject.so";
        void *handle = load_module(inject_library_path);
        char *funcName = "init_symbol";
        char *moduleName = "libinject.so";
        unsigned long inject_func_offset = get_func_offset(funcName,moduleName,handle);
       
       	printf("inject_func_offset:0x%lx\n", inject_func_offset);
	
	return 0;
}
