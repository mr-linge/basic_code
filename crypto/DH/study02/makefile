cc=clang

dylib:
	@$(cc) -shared -fPIC dh_exchange.c -o libdh.so

client:
	@$(cc) random_prime.c dh_exchange.c client.c -o client

server:
	@$(cc) random_prime.c dh_exchange.c server.c -o server

clean:
	@rm -rf client server test
