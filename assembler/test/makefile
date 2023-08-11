# base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/linux-x86_64/bin
base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/darwin-x86_64/bin
cc=${base_dir}/aarch64-linux-android33-clang

# 生成动态链接库
dylib:
	@$(cc) -S caculate.c -o caculate.asm
	@$(cc) -c caculate.asm -o caculate.o
	@$(cc) -fPIC -shared caculate.o -o libcaculate.so

program:
	@$(cc) -S main.c -o main.asm
	@$(cc) -c main.asm -o main.o
	@$(cc) main.o -o main -ldl

all: dylib program
	@adb push libcaculate.so /data/local/tmp/
	@adb push main /data/local/tmp/

clean:
	@-rm -rf *.o main.asm main
