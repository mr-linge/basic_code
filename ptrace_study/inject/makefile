# base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/linux-x86_64/bin
base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/darwin-x86_64/bin
cc=${base_dir}/aarch64-linux-android33-clang
strip=${base_dir}/llvm-strip

tracee:
	@$(cc) tracee.c -o tracee
	@adb push ./tracee /data/local/tmp/

inject:
	@$(cc) inject.c address.c ptrace_util.c -o inject
	@adb push ./inject /data/local/tmp/

dylib:
	@${cc} -shared -fPIC test.c -o liba.so
	@adb push ./liba.so /data/local/tmp/

test2:
	@$(cc) test2.c -o test2
	@adb push ./test2 /data/local/tmp/

test3: dylib
	@$(cc) test3.c address.c -o test3
	@adb push ./test3 /data/local/tmp/

all: tracee inject

push:
	@adb push ./liba.so /data/local/tmp/

clean:
	@-rm -rf ./tracee *.so test2 test3 inject
