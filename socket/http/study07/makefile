cc = gcc

server:
	@$(cc) main.c server.c service.c http_parse.c -o server

client:
	@$(cc) client.c -o client

all: server client

clean:
	@-rm -rf server client
