cc=gcc

# 使用到的函数没有在libc库中,那么你就需要指定要链接的库, #include <math.h> 需要链接libm.so或libm.a
# -l 		指定函数库名,其中的lib和.a(.so)省略。 -lm	参数表明链接libm.a这个库(类似的,如果要链接libxxx.a,使用-lxxx即可)。
# -static 	链接器应该使用静态链接(默认使用的是动态链接)

# 静态库会将libm.a中需要的代码"拷贝"到最终的可执行文件main中
static_link:
	${cc} -static main.c -o main -lm

all: static_link
	./main

clean:
	@rm -rf ./main
