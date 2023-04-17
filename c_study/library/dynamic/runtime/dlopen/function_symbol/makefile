cc = gcc

dylib: caculate.c
	@$(cc) -fPIC -shared caculate.c -o libcaculate.so

all: dylib
	@$(cc) -rdynamic main.c -o main -ldl
	@./main

clean:
	@-rm ./main ./libcaculate.so
