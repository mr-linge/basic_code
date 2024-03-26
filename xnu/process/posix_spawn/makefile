cc=clang

# 架构
arch=arm64

main:
	@${cc} -arch ${arch} main.m -o main

target:
	@${cc} -arch ${arch} target.c -o target

sign:
	@/usr/bin/codesign --force --sign - --entitlements entitlements.plist main
	@/usr/bin/codesign --force --sign - --entitlements entitlements.plist target

all: main target sign

clean:
	@-rm -rf ./main ./target
