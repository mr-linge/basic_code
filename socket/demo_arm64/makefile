base_dir = /home/dio/Development/Android/Sdk/ndk/20.0.5594570/toolchains/llvm/prebuilt/linux-x86_64/bin/
cc = $(base_dir)aarch64-linux-android29-clang
strip = $(base_dir)aarch64-linux-android-strip


client_exe = client
client_source = client.c

client:
	$(cc) $(client_source) -o $(client_exe)
	@adb push ./$(client_exe) /data/local/tmp/work
	@adb shell "chmod +x /data/local/tmp/work/$(client_exe)"

server: server.c
	gcc server.c -o server #-lpthread

clean:
	@-rm -rf client server
