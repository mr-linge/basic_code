cc=gcc

program:
	@$(cc) main.c -o main
	@./main `pidof ./main` libtest.so

clean:
	@-rm -rf  main