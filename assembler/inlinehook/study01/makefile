# base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/linux-x86_64/bin
base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/darwin-x86_64/bin
cc=$(base_dir)/aarch64-linux-android33-clang

source=main.c
outfile=main

library_source=hook.c
lib_outfile=libhook.so

program:
	@$(cc) ${source} -o $(outfile)

dylib:
	@$(cc) -fPIC -shared ${library_source} -o $(lib_outfile)

test:
	@$(cc) -fPIC -shared hook.c test.c -o libtest.so

all: program test
	@adb push $(outfile) /data/local/tmp/
	@adb push libtest.so /data/local/tmp/

clean:
	@-rm -rf libtest.so $(outfile) $(lib_outfile)
