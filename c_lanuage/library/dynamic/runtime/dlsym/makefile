# base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/linux-x86_64/bin
base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/darwin-x86_64/bin
cc=${base_dir}/aarch64-linux-android33-clang
strip=${base_dir}/llvm-strip

tracee:
	@$(cc) dlsym01.c -o main -ldl

all: tracee
	@$(strip) ./main
	@adb push ./main /data/local/tmp/
	@adb push ./loaded_file /data/local/tmp/
	
clean:
	@-rm -rf ./main
