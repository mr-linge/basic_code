cc = gcc

# 生成动态链接库
dylib:
	@$(cc) -fPIC -shared global_var.c -o libglobal.so

program: 
	@$(cc) -rdynamic main.c -o main -ldl

all: dylib program
	@./main

clean:
	@-rm -rf ./main ./libglobal.so
