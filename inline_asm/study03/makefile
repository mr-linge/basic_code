# base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/linux-x86_64/bin
base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/darwin-x86_64/bin
cc=${base_dir}/aarch64-linux-android33-clang

source=main.c
outfile=main

program:
	@$(cc) $(source) -o $(outfile)

all: program
	@adb push $(outfile) /data/local/tmp/

clean:
	@-rm -rf $(outfile) $(lib_outfile)
