cc = gcc

# 生成动态链接库
dylib: 
	@$(cc) -shared -fPIC -nostartfiles -o libtest.so test.c

program: 
	@$(cc) -rdynamic main.c -o main -ldl

all: dylib program
	@./main

clean:
	@-rm -rf ./main *.so
