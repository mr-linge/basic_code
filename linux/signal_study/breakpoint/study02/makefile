# base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/linux-x86_64/bin
base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/darwin-x86_64/bin
cc=$(base_dir)/aarch64-linux-android33-clang

program:
	@$(cc) -S main.c -o main.asm
	@$(cc) -c main.asm -o main.o
	@$(cc) -c func.asm -o func.o
	@$(cc) main.o func.o -o main

push:
	@adb push ./main /data/local/tmp/

all: program push

clean:
	@-rm -rf *.o main.asm ./main
