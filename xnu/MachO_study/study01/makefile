cc=clang

framework=Foundation
memory_mode=-fobjc-arc
# 架构
arch=arm64
# -isysroot指定系统SDK路径
SDK_path=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk


exec_source=main.m parse.m
exec_outfile=main

dylib:
	@${cc} ${memory_mode} -arch ${arch} -framework ${framework} -fPIC -shared ./demo/caculate.c -o libcaculate.dylib

exec:
	@${cc} ${memory_mode} -arch ${arch} -framework ${framework} ./demo/exec_code.c -o exec_code

main:
	@${cc} ${memory_mode} -arch ${arch} -framework ${framework} -isysroot ${SDK_path} ${exec_source} -o ${exec_outfile}

sign:
	@/usr/bin/codesign --force --sign - --entitlements entitlements.plist ${exec_outfile}

all: main sign dylib exec

clean:
	@-rm -rf ${exec_outfile} libcaculate.dylib exec_code
