cc=gcc

# 使用到的函数没有在libc库中，那么你就需要指定要链接的库, #include <math.h>需要链接libm.so或libm.a
# -l 指定函数库名，其中的lib和.a(.so)省略。-lm	参数表明链接libm.so这个库（类似的，如果要链接libxxx.so,使用-lxxx即可）.

# 动态库不在链接时将需要的二进制代码都“拷贝”到可执行文件中，而是仅仅“拷贝”一些重定位和符号表信息，这些信息可以在程序运行时完成真正的链接过程
dynamic_link:
# 默认使用的是动态链接
	${cc} main.c -o main -lm


exec: dynamic_link 
	./main


clean:
	@rm -rf ./main
