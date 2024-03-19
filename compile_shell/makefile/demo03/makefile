cc = gcc

so = libcaculate.so
exec = main

$(so): caculate.c
	@$(cc) -fPIC -shared caculate.c -o $(so)

$(exec): main.c
	@$(cc) -rdynamic main.c -o $(exec) -ldl

all: $(exec) $(so)
	@./$(exec)

.PHONY: clean
clean:
	@-rm -rf $(so) $(exec)
