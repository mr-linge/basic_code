cc=gcc

libcaculate.so: caculate.c
	@ $(cc) -fPIC -shared caculate.c -o libcaculate.so

main: main.c
	@ $(cc) -rdynamic main.c -o main -ldl

all: main libcaculate.so
	@ ./main

.PHONY: clean
clean:
	@-rm -rf libcaculate.so main
