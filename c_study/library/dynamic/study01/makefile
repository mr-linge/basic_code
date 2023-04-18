cc=gcc

# 将.c 编译生成 .o文件, 无论是静态库文件还是动态库文件，都是由 .o 文件创建的
object:
	${cc} -c add.c -o add.o
	${cc} -c sub.c -o sub.o


# 由 .o创建.so动态库
# 动态库文件名命名规范和静态库文件名命名规范类似，也是在动态库名增加前缀lib，但其文件扩展名为.so
# -fPIC:产生位置无关代码
# -shared:生成共享库
dylib:
	${cc} -shared -fPIC -o libmymath.so add.o sub.o


# 在程序中使用静态库
# -L 指定函数库查找的位置，注意L后面还有'./',表示在当前目录下查找
# -l 指定函数库名，其中的lib和.a(.so)省略。
program: object dylib
	${cc} -o main main.c -L./ -lmymath


exec: program
# 环境变量LD_LIBRARY_PATH指定动态库搜索路径(在 shell 里执行)
#	export LD_LIBRARY_PATH=./
# 默认的动态库搜索路径 /lib  /usr/lib
	 sudo mv libmymath.so /usr/lib
	./main


clean:
	@-rm -rf *.o *.so ./main
	@-sudo rm -rf /usr/lib/libmymath.so
