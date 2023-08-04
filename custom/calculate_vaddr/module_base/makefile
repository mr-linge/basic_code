cc=gcc

source=module_base_addr.c

program:
	@$(cc) ${source} -o main
	@./main `pidof ./tracee` libtest.so

clean:
	@-rm -rf main
