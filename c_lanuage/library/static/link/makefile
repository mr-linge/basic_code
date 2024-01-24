cc=gcc

# -static 	链接时使用静态链接(默认使用的是动态链接)

# 静态链接 会将 静态库(libm.a) 中需要的代码"转移"到最终的可执行文件中
static_link:
	${cc} -static main.c -o main -lm

all: static_link
	./main

clean:
	@-rm -rf ./main
