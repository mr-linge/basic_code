# base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/linux-x86_64/bin
base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/darwin-x86_64/bin
cc=$(base_dir)/aarch64-linux-android33-clang

source=main.c
outfile=main

library_source=hook.c
lib_outfile=libhook.so

program:
	@$(cc) -c func.asm -o func.o
	@$(cc) -S main.c -o main.asm
	@$(cc) -c main.asm -o main.o
	@$(cc) main.o func.o -o $(outfile)

dylib:
	@$(cc) -fPIC -shared ${library_source} -o $(lib_outfile)

push:
	@adb push $(outfile) /data/local/tmp/

push_dylib:
	@adb push $(lib_outfile) /data/local/tmp/


program_all: program push


all: dylib push_dylib


clean:
	@-rm -rf *.o $(outfile) $(lib_outfile)
