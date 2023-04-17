cc=gcc

static_library_source=test.c
static_library_object=test.o
static_library=libadd.a

source=main.c
source_object=main.o
source_exec=main

# 编译成 object file
static_object: ${static_library_source}
	${cc} -c ${static_library_source} -o ${static_library_object}

# 将 object file 进行压缩，压缩后的文件就是 static library
static_ar: ${static_library_object}
	ar -r ${static_library} ${static_library_object}

static_library: static_object static_ar


# 将源代码先编译成 object file
main_o: ${source}
	${cc} -c ${source} -o ${source_object}

# 将源代码先编译成 object file 和 static library 里的 object file 链接成 可执行文件
main_link: ${source_object}
	${cc} ${source_object} -L. -ladd -o main


main_exec: main_o main_link


all: static_library main_exec


clean:
	rm -rf ${static_library_object} ${static_library} ${source_object} ${source_exec}
