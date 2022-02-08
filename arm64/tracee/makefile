base_dir = /usr/local/android/sdk/ndk/20.0.5594570/toolchains/llvm/prebuilt/linux-x86_64/bin/
cc = $(base_dir)aarch64-linux-android29-clang
strip = $(base_dir)aarch64-linux-android-strip

.so = libcaculate.so
exec = tracee

all: $(exec)
	# @$(strip) ./$(exec)
	@adb push ./$(exec) /data/local/tmp/work
	@adb shell "chmod +x /data/local/tmp/work/$(exec)"

$(exec): tracee.c
	@ $(cc) -fPIE tracee.c -o $(exec) -ldl

so : caculate.c
	@ $(cc) -fPIC -shared caculate.c -o $(.so)

.PHONY:clean
clean:
	@-rm $(exec) $(.so)
