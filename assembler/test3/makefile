base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/linux-x86_64/bin
# base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/darwin-x86_64/bin
cc=${base_dir}/aarch64-linux-android33-clang


dylib:
	@$(cc) -S add.c -o add.asm
	@${cc} -c add.asm -o add.o
	@${cc} -shared -fPIC -o libmymath.so add.o


dylib_from_asm:
	@${cc} -c add.asm -o add.o
	@${cc} -shared -fPIC -o libmymath.so add.o
	@adb push libmymath.so /data/local/tmp/


program:
	@$(cc) -S main.c -o main.asm
	@${cc} -c main.asm -o main.o
	@${cc} -o main main.o -L./ -lmymath


all: dylib program
	@adb push libmymath.so /data/local/tmp/
	@adb push main /data/local/tmp/


clean:
	@-rm -rf *.o *.asm *.so ./main
