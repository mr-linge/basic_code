cc = gcc

server: server.c
	@$(cc) server.c -o server
	@./server

client: client.c
	@$(cc) client.c -o client
	@./client

clean:
	@-rm server client
	@clear
