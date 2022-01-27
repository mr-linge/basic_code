cc = gcc

so = libcaculate.so
exec = main

all: $(exec) $(so)
	@ ./$(exec)

$(exec): main.c
	@ $(cc) -rdynamic main.c -o $(exec) -ldl

$(so): caculate.c
	@ $(cc) -fPIC -shared caculate.c -o $(so)

.PHONY: clean
clean:
	clear
	@ - rm $(so) $(exec)
