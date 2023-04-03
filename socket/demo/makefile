cc = clang

client: client.c
	$(cc) client.c -o client

server: server.c
	$(cc) server.c -o server #-lpthread

all: clean client server

clean:
	@- rm -rf client server
