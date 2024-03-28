cc=clang

framework=Foundation
# 内存管理模式
memory_mode=-fobjc-arc
# 架构
arch=arm64
# -isysroot指定系统SDK路径
SDK_path=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk


source=main.m
outfile=main

main:
	@${cc} ${memory_mode} -arch ${arch} -framework ${framework} -isysroot ${SDK_path} ${source} -o ${outfile}

# 签名 Mach-O 可执行文件
sign: 
	@/usr/bin/codesign --force --sign - ${outfile}

all: main sign

clean:
	@-rm -rf $(outfile)
