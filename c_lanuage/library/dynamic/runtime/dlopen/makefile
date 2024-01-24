cc = gcc

# 生成动态库
dylib:
	@$(cc) -fPIC -shared caculate.c -o libcaculate.so

main: 
	@$(cc) main.c -o main -ldl

all: dylib main
	@./main

clean:
	@-rm ./main ./libcaculate.so
