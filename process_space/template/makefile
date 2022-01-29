cc = gcc

exec = main

source = vaddr_by_symbol.c main2.c

all:$(exec)
	@./$(exec)

$(exec):$(source)
	@$(cc) -no-pie $(source) -o $(exec) -ldl

clean:
	@-rm ./$(exec)
