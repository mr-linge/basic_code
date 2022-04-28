base_dir = /home/dio/Development/Android/Sdk/ndk/20.0.5594570/toolchains/llvm/prebuilt/linux-x86_64/bin/
cc = $(base_dir)aarch64-linux-android29-clang
strip = $(base_dir)aarch64-linux-android-strip

exec = main

source = main2.c ptrace_util.c vaddr_by_symbol.c

source3 = main3.c hook_util.c ptrace_util.c vaddr_by_symbol.c 


main3: $(source3)
	@$(cc) $(source3) -o $(exec)
	@adb push ./$(exec) /data/local/tmp/work
	@adb shell "chmod +x /data/local/tmp/work/$(exec)"

all: $(exec)
	@$(strip) ./$(exec)
	@adb push ./$(exec) /data/local/tmp/work
	@adb shell "chmod +x /data/local/tmp/work/$(exec)"

$(exec): $(source)
	@-rm $(exec)
	@$(cc) $(source) -o $(exec)

clean:
	@-rm $(exec)
