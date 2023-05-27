base_dir = /home/me/Library/Android/Sdk/ndk/20.0.5594570/toolchains/llvm/prebuilt/linux-x86_64/bin/
cc = $(base_dir)aarch64-linux-android29-clang
strip = $(base_dir)aarch64-linux-android-strip


client_exe = client
client_source = client.c

client:
	@$(cc) $(client_source) -o client

server: server.c
	@$(cc) server.c -o server #-lpthread

push:
	@adb push ./client /data/local/tmp
	@adb push ./server /data/local/tmp

all: client server


clean:
	@-rm -rf client server
