# cc=gcc
base_dir=/home/me/Library/Android/Sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/linux-x86_64/bin
cc=${base_dir}/aarch64-linux-android29-clang
strip=${base_dir}/aarch64-linux-android-strip

loader:
	@$(cc) loader.c -o load

test:
	@$(cc) test.c -o test

push:
	adb push ./test /data/local/tmp
	adb push ./load /data/local/tmp
	adb push ./config.ini /data/local/tmp
	
clean:
	@-rm -rf load *.log