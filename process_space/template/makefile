cc = gcc

exec = main

source = vaddr_by_symbol.c main.c

all:$(exec)
	@./$(exec)

$(exec):$(source)
	@$(cc) -no-pie $(source) -o $(exec)

clean:
	@-rm ./$(exec)
