cc=gcc


object:
	@${cc} -c add.c -o add.o

dylib:
	@${cc} -shared -fPIC -o libmymath.so add.o


program: object dylib
	@${cc} -o main main.c -L./ -lmymath


exec: program
	@sudo mv libmymath.so /usr/lib
	@./main


clean:
	@-rm -rf *.o *.so ./main
	@-sudo rm -rf /usr/lib/libmymath.so
