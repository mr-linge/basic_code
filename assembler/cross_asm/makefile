# base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/linux-x86_64/bin
base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/darwin-x86_64/bin
cc=${base_dir}/aarch64-linux-android33-clang

asm:
	@$(cc) -S add.c -o add.asm

program:
	@$(cc) -c add.asm -o add.o
	@$(cc) -S main.c -o main.asm
	@$(cc) -c main.asm -o main.o
	@$(cc) main.o add.o -o main

all: program
	@adb push $(outfile) /data/local/tmp/

clean:
	@-rm -rf *.o main.asm main
