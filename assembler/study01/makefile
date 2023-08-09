base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/linux-x86_64/bin
# base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/darwin-x86_64/bin
cc=${base_dir}/aarch64-linux-android33-clang


outfile=main

program:
	@$(cc) -c main.asm -o main.o
	@$(cc) main.o -o main

all: program
	@adb push $(outfile) /data/local/tmp/

push:
	@adb push $(outfile) /data/local/tmp/

clean:
	@-rm -rf *.o main
