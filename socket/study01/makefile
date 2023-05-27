cc = clang

client:
	@$(cc) client.c -o client

server:
	@$(cc) server.c -o server #-lpthread

all: client server

clean:
	@-rm -rf client server
