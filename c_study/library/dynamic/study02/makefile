cc=gcc

# 使用到的函数没有在libc库中，那么你就需要指定要链接的库, #include <math.h>需要链接libm.so或libm.a
# -l 指定函数库名，其中的lib和.a(.so)省略。

# 详细说明: -lm	参数表明链接libm.so这个库（类似的，如果要链接libxxx.so,使用-lxxx即可）
dynamic_link:
# 默认使用的是动态链接
	${cc} main.c -o main -lm


exec: dynamic_link 
	./main


clean:
	@rm -rf ./main
