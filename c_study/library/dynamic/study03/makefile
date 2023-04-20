cc=gcc


# 先编译一个静态库
static_library:
	${cc} -c add.c -o add.o
	${cc} -c sub.c -o sub.o
	ar -cr libmymath.a add.o sub.o


# 编译的动态库需要引用另一个静态库
# 把静态库里的 object file 与 动态库的 object file链接成一个动态库
dynamic_library: static_library
	 ${cc} -c func.c -o func.o
	 ${cc} -shared -fPIC func.o libmymath.a -o libmymath.so
	 @rm -rf libmymath.a

# 	下面这种方式 linux clang 是支持的
#	 ${cc} -c func.c -o func.o
#	 ${cc} -shared -fPIC -static func.o -o libmymath.so -L./ -lmymath
#	 @rm -rf libmymath.a


# -L 指定依赖库查找的位置，注意L后面还有'./',表示在当前目录下查找
# -l 指定依赖库名，其中的lib和.a(.so)省略。
program: dynamic_library
	${cc} -o main main.c -L./ -lmymath


all: program
# 环境变量LD_LIBRARY_PATH指定动态库搜索路径(在 shell 里执行)
	export LD_LIBRARY_PATH=./
# 默认的动态库搜索路径 /lib  /usr/lib
#	 sudo mv libmymath.so /usr/lib
	./main


clean:
	@-rm -rf *.o *.so ./main
	@-sudo rm -rf /usr/lib/libmymath.so
