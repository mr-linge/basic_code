cc=clang

static_library_source=test.c
static_library_object=test.o
static_library=libadd.a

source=main.c
source_object=main.o
source_exec=main

static_object: ${static_library_source}
	${cc} -c ${static_library_source} -o ${static_library_object}

static_ar: ${static_library_object}
	ar -r ${static_library} ${static_library_object}

static_library: static_object static_ar


main_o: ${source}
	${cc} -c ${source} -o ${source_object}

main_link: ${source_object}
	${cc} ${source_object} -L. -ladd -o main

main_exec: main_o main_link

clean:
	rm -rf ${static_library_object} ${static_library} ${source_object} ${source_exec}
