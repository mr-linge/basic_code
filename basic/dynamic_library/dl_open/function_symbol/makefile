cc = gcc

all: main.c
	@$(cc) -rdynamic main.c -o main -ldl
	@./main

so: caculate.c
	@$(cc) -fPIC -shared caculate.c -o libcaculate.so

clean:
	@-rm ./main
