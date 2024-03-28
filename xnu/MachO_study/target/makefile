cc=clang

# 架构
arch=arm64
# -isysroot指定系统SDK路径
SDK_path=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk

exec_source=exec_file.m
exec_outfile=exec_file

dylib_source=caculate.c
dylib_outfile=libcaculate.dylib

# 生成动态库
dylib:
	@$(cc) -fobjc-arc -fmodules -arch ${arch} -isysroot ${SDK_path} -fPIC -shared ${dylib_source} -o ${dylib_outfile}

exec: 
	@$(cc) -fobjc-arc -fmodules -arch ${arch} -isysroot ${SDK_path} ${exec_source} -o ${exec_outfile}

sign:
	@/usr/bin/codesign -f -s - --entitlements entitlements.plist ${dylib_outfile}
	@/usr/bin/codesign -f -s - --entitlements entitlements.plist ${exec_outfile} 

all: dylib exec sign

clean:
	@-rm -rf ${exec_outfile} ${dylib_outfile}
