cc=gcc

# -static 	链接静态库(默认链接动态库)

# 链接静态库 会将 静态库(libm.a) 中需要的代码"转移"到最终的可执行文件中
static_link:
	${cc} -static main.c -o main -lm

all: static_link
	./main

clean:
	@-rm -rf ./main
