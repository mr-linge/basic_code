
all:parser
	@./parser ./target

parser:
	@gcc -no-pie parser.c -o parser

target:
	@gcc -no-pie my.c target.c -o target

clean:
	@-rm ./parser
	@clear
