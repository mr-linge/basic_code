cc = gcc

all: main libcaculate.so
	@ ./main

main: main.c
	@ $(cc) -rdynamic main.c -o main -ldl

libcaculate.so: caculate.c
	@ $(cc) -fPIC -shared caculate.c -o libcaculate.so

.PHONY: clean
clean:
	clear
	@ - rm libcaculate.so main
