cc=clang

# 架构
arch=arm64
# -isysroot指定系统SDK路径
SDK_path=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk

source=main.m
outfile=dump

main:
	@${cc} -fobjc-arc -fmodules -arch ${arch} -isysroot ${SDK_path} ${source} -o ${outfile}
	@/usr/bin/codesign -f -s - --entitlements entitlements.plist ${outfile} 

dylib:
	@${cc} -fobjc-arc -fmodules -arch ${arch} -isysroot ${SDK_path} -fPIC -shared dumpdecrypted.c -o libdump.dylib
	@/usr/bin/codesign -f -s - --entitlements entitlements.plist libdump.dylib

all: dylib

clean:
	@-rm -rf ${outfile}
