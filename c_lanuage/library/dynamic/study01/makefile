cc=clang

# 将 .c(source file) 编译生成 .o (object file), 无论是静态库还是动态库,都是由 .o文件 创建的
object:
	${cc} -c add.c -o add.o
	${cc} -c sub.c -o sub.o


# 由 .o 创建 .so(动态库)
# 动态库文件名命名规范和静态库文件名命名规范类似,前缀lib,文件扩展名为.so
# -fPIC			产生位置无关代码
# -shared		生成共享库
dylib: object
	${cc} -shared -fPIC add.o sub.o -o libmymath.so


# -L  					编译时指定依赖库搜寻目录,如指定当前目录 -L./
# -l    				编译时指定要链接的依赖库的名称,如库的名称是libtest.a,则 -ltest 即去头去尾
# -Wl,-rpath=<path> 	编译时指定运行时动态库搜索路径
main: object dylib
#	${cc} -Wl,-rpath=./ -o main main.c -L./ -lmymath
	${cc} -o main main.c -L./ -lmymath


all: main
# 环境变量LD_LIBRARY_PATH指定动态库搜索路径(在 shell 里执行)
	export LD_LIBRARY_PATH=./
# 默认的动态库搜索路径 /lib/  /usr/lib/  /usr/local/lib/
#	 sudo mv libmymath.so /usr/lib/
	./main


clean:
	@-rm -rf *.o *.so ./main
	@-sudo rm -rf /usr/local/lib/libmymath.so /usr/lib/libmymath.so
