cc=clang

# 架构
arch=arm64
# -isysroot指定系统SDK路径
SDK_path=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk

source=main.m
outfile=main

main:
	@${cc} -fobjc-arc -fmodules -arch ${arch} -isysroot ${SDK_path} ${source} -o ${outfile}
	@/usr/bin/codesign -f -s - ${outfile} 

all: main

clean:
	@-rm -rf ${outfile}
