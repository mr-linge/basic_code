cc = gcc

client: client.c
	$(cc) client.c -o client

server: server.c
	$(cc) server.c -o server #-lpthread

clean:
	@- rm -rf client server
