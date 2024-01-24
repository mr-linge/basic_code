cc=gcc

# -static 	链接静态库(不加-static 同时有动态库和静态库时优先链接动态库)

# 链接静态库 会将 静态库(libm.a) 中需要的代码"整合"到最终的可执行文件中
static_link:
	${cc} -static main.c -o main -lm

all: static_link
	./main

clean:
	@-rm -rf ./main
