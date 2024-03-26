cc=clang

# 架构
arch=arm64
# -isysroot指定系统SDK路径
SDK_path=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk

exec_source=main.m parse.m
exec_outfile=main

# 生成动态库
dylib:
	@$(cc) -fobjc-arc -fmodules -arch ${arch} -isysroot ${SDK_path} -fPIC -shared caculate.c -o libcaculate.dylib

main: 
	@$(cc) -fobjc-arc -fmodules -arch ${arch} -isysroot ${SDK_path} ${exec_source} -o ${exec_outfile} -ldl

sign:
	@/usr/bin/codesign -f -s - --entitlements entitlements.plist libcaculate.dylib
	@/usr/bin/codesign -f -s - --entitlements entitlements.plist main

all: dylib main sign

clean:
	@-rm -rf ${exec_outfile} ./libcaculate.dylib
