cc=gcc

dylib_source=aes128_crypt.c aes128_blcok_crypt.c
program_source=aes128_crypt.c aes128_blcok_crypt.c main.c

dylib:
	$(cc) -shared -fPIC $(dylib_source) -o libcrypto.so

exec:
	$(cc) $(program_source) -o main

clean:
	@-rm -rf *.so main
