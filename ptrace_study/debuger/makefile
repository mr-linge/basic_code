# base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/linux-x86_64/bin
base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/darwin-x86_64/bin
cc=${base_dir}/aarch64-linux-android33-clang
strip=${base_dir}/llvm-strip

source=main.c ptrace_util.c breakpoint.c console.c
execfile=debuger

program:
	@$(cc) $(source) -o $(execfile)

test:
	@clang test.c -o test

all: program
	@$(strip) $(execfile)
	@adb push $(execfile) /data/local/tmp/

clean:
	@-rm -rf $(execfile) ./test
