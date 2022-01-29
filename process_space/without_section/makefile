cc = gcc

exec = main

source = main2.c

all:$(exec)
	@./$(exec) `pidof ./tracee`

$(exec): $(source)
	@$(cc) -no-pie $(source) -o $(exec) -ldl

clean:
	@-rm $(exec)
