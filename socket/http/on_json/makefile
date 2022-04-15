cc = gcc

server: server.c
	@$(cc) server.c cJSON.c cJSON_Utils.c -o server

client: client.c
	@$(cc) client.c cJSON.c cJSON_Utils.c -o client

clean:
	@-rm server client
