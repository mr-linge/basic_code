cc = gcc

dylib_source = test.c

program: main.c
	@$(cc) -rdynamic main.c -o main -ldl
	@./main

dylib: $(.so)
	@$(cc) -fPIC -shared $(dylib_source) -o libtest.so

all: program dylib


clean:
	@-rm ./main
