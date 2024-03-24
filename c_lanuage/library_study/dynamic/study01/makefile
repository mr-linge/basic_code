cc=gcc

# 链接动态库时 获取动态库里一些重定位和符号表信息,这些信息可以在程序运行时完成符号地址的计算
dynamic_link:
	${cc} main.c -o main -lm

all: dynamic_link
	./main

clean:
	@-rm -rf ./main
