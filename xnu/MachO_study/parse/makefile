cc=clang

framework=Foundation
# 架构
arch=arm64
# -isysroot指定系统SDK路径
SDK_path=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk

test1_source=test1.m parse.m
test1_outfile=test1

test2_source=test2.m parse.m
test2_outfile=test2

test3_source=test3.m parse.m
test3_outfile=test3

test1:
	@${cc} -fobjc-arc -framework ${framework} -arch ${arch} -isysroot ${SDK_path} ${test1_source} -o ${test1_outfile}
	@/usr/bin/codesign -f -s - ${test1_outfile} 

test2: 
	@$(cc) -fobjc-arc -framework ${framework} -arch ${arch} -isysroot ${SDK_path} ${test2_source} -o ${test2_outfile} -ldl
	@/usr/bin/codesign -f -s - ${test2_outfile} 

test3: 
	@$(cc) -fobjc-arc -framework ${framework} -arch ${arch} -isysroot ${SDK_path} ${test3_source} -o ${test3_outfile} -ldl
	@/usr/bin/codesign -f -s - ${test3_outfile} 

all: test1 test2 test3

clean:
	@-rm -rf ${test1_outfile} ${test2_outfile} ${test3_outfile} 
