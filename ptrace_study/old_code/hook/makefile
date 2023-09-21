base_dir = /home/dio/Development/Android/Sdk/ndk/20.0.5594570/toolchains/llvm/prebuilt/linux-x86_64/bin/
cc = $(base_dir)aarch64-linux-android29-clang
strip = $(base_dir)aarch64-linux-android-strip

exec = main

source3 = main3.c hook_util.c ptrace_util.c vaddr_by_symbol.c 

ssl_source = hook_util.c ptrace_util.c vaddr_by_symbol.c android_ssl.c

android_source = hook_util.c ptrace_util.c vaddr_by_symbol.c android_so.c


main3: $(source3)
	@$(cc) $(source3) -o $(exec)
	@adb push ./$(exec) /data/local/tmp/work
	@adb shell "chmod +x /data/local/tmp/work/$(exec)"

ssl: $(ssl_source)
	@$(cc) $(ssl_source) -o $(exec)
	@$(strip) ./$(exec)
	@adb push ./$(exec) /data/local/tmp/work
	@adb shell "chmod +x /data/local/tmp/work/$(exec)"

android:$(android_source)
	@$(cc) $(android_source) -o $(exec)
	@$(strip) ./$(exec)
	@adb push ./$(exec) /data/local/tmp/work
	@adb shell "chmod +x /data/local/tmp/work/$(exec)"

clean:
	@-rm $(exec)
