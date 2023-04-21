cc = gcc

# 生成动态链接库
dylib: 
	@$(cc) -shared -fPIC -nostartfiles -o libtest.so test.c
#	@$(cc) -fPIC -shared test.c -o libtest.so

program: 
	@$(cc) -rdynamic main.c -o main -ldl

all: dylib program
	@./main

clean:
	@-rm -rf ./main *.so
