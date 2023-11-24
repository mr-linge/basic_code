cc = gcc

# 生成动态链接库
dylib: caculate.c
	@$(cc) -fPIC -shared caculate.c -o libcaculate.so

program: 
	@$(cc) main.c -o main -ldl

all: dylib program
	@./main

clean:
	@-rm ./main ./libcaculate.so
