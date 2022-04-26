cc = gcc

.so = test.c

all: main.c
	@$(cc) -rdynamic main.c -o main -ldl
	@./main

so: $(.so)
	@$(cc) -fPIC -shared $(.so) -o libtest.so

clean:
	@-rm ./main
