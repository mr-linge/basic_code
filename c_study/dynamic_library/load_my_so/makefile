cc=gcc

dynamic_source=test.c
dynamic_library=libtest.so

main_source=main.c
main_exec=main

# 编译动态链接库
dylib:
	${cc} -fPIC -shared ${dynamic_source} -o ${dynamic_library}

# 编译可执行文件时指定动态库搜索目录
main_exec:
	@# -l  选项告诉编译器要使用test这个库
	@# -L. 告诉编译器在当前目录中查找库文件
	${cc} -no-pie ${main_source} -o ${main_exec} -L. -ltest

# 运行可执行文件
all: dylib main_exec
	@#  把当前路径加入环境变量LD_LIBRARY_PATH中
	export LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH}
	./${main_exec}

clean:
	rm -rf ${dynamic_library} ${main_exec}
