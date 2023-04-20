cc=clang

# 将.c 编译生成 .o文件, 无论是静态库文件还是动态库文件，都是由 .o 文件创建的
object:
	${cc} -c add.c -o add.o
	${cc} -c sub.c -o sub.o


# 由 .o创建.so动态库
# 动态库文件名命名规范和静态库文件名命名规范类似，也是在动态库名增加前缀lib，但其文件扩展名为.so
# -fPIC:产生位置无关代码
# -shared:生成共享库
dylib: object
	${cc} -shared -fPIC -o libmymath.so add.o sub.o


# -L 指定依赖库查找的位置，注意L后面还有'./',表示在当前目录下查找
# -l 指定依赖库名，其中的lib和.a(.so)省略。
program: object dylib
	${cc} -o main main.c -L./ -lmymath


all: program
# 环境变量LD_LIBRARY_PATH指定动态库搜索路径(在 shell 里执行)
	export LD_LIBRARY_PATH=./
# 默认的动态库搜索路径 /lib  /usr/lib  /usr/local/lib
#	 sudo mv libmymath.so /usr/lib
	./main


clean:
	@-rm -rf *.o *.so ./main
	@-sudo rm -rf /usr/local/lib/libmymath.so /usr/lib/libmymath.so
