cc=gcc

source=main.c
dylib_source=test.c

dylib: test.c
	@$(cc) -fPIC -shared $(dylib_source) -o libtest.so

program: main.c
	@$(cc) -no-pie main.c -o tracee -L. -ltest -ldl

all: dylib program
	@# export LD_LIBRARY_PATH=./
	./tracee

clean:
	@-rm -rf tracee libtest.so
