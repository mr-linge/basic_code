cc = gcc

client:
	$(cc) -o client client_ssl.c -Wall -lssl -ldl -lcurses -lcrypto -g

server:
	$(cc) -o server server_ssl.c -Wall -lssl -ldl -lcurses -lcrypto -g

clear:
	@- rm -rf client server
