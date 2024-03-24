# base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/linux-x86_64/bin
base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/darwin-x86_64/bin
cc=${base_dir}/aarch64-linux-android33-clang
strip=${base_dir}/llvm-strip


tracee:
	@$(cc) tracee.c -o tracee
	@$(strip) tracee

tracee2:
	@$(cc) tracee2.c -o tracee
	@$(strip) tracee

tracee4:
	@$(cc) tracee4.c -o tracee
	@$(strip) tracee

all: tracee4
	@adb push tracee /data/local/tmp/

clean:
	@-rm -rf tracee
