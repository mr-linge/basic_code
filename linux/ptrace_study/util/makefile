# base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/linux-x86_64/bin
base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/darwin-x86_64/bin
cc=${base_dir}/aarch64-linux-android33-clang
strip=${base_dir}/llvm-strip

tracee:
	@$(cc) tracee.c -o tracee
	@adb push ./tracee /data/local/tmp/

program:
	@$(cc) main.c ptrace_util.c address.c -o main
	@adb push ./main /data/local/tmp/

dylib:
	@${cc} -shared -fPIC test.c -o liba.so
	@adb push ./liba.so /data/local/tmp/

all: tracee program

clean:
	@-rm -rf *.so ./tracee ./main
