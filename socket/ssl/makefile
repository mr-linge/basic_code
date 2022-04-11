cc = gcc

client:
	$(cc) -Wall client.c -lssl -lcrypto -o client

server:
	$(cc) -Wall server.c -lssl -lcrypto -o server

clean:
	@- rm -rf client server
