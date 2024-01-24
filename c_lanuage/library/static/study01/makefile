cc=clang

# 将 .c(source file) 编译生成 .o(object file), 无论是静态库文件还是动态库文件, 都是由 .o 文件创建的
object:
	${cc} -c add.c -o add.o
	${cc} -c sub.c -o sub.o


# 由 .o(object file) 创建 .a静态库
# ar : archive 压缩, object file 压缩成 static library
# -c : create 创建
# -r : replace 替换,表示当前插入的模块名已经在库中存在,则替换同名的模块。如果若干模块中有一个模块在库中不存在,ar显示一个错误信息,并不替换其他同名的模块。默认的情况下,新的成员增加在库结尾处。
# 静态依赖库文件的命名规范: 前缀lib, 扩展名 .a
ar: object
	ar -cr libmymath.a sub.o add.o


# 在程序中使用静态库
# -L 指定函数库查找的位置,注意L后面还有'./',表示在当前目录下查找
# -l 指定函数库名,其中的lib和.a(.so)省略
all: object ar
	${cc} -o main main.c -L./ -lmymath


clean:
	@-rm -rf *.o *.a ./main
