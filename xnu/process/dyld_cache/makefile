cc=clang

arch=arm64
framework=Foundation
SDK_path=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk

source=main.m
outfile=main

main:
	@${cc} -fobjc-arc -framework ${framework} -arch ${arch} -isysroot ${SDK_path} ${source} -o ${outfile}
	@/usr/bin/codesign -f -s - ${outfile} 

all: main

clean:
	@-rm -rf ${outfile}
