cc=clang

# 架构
arch=arm64

main:
	@${cc} -arch ${arch} main.c -o main
	@/usr/bin/codesign --force --sign - --entitlements entitlements.plist main
	@scp -r -P 2222 ./main root@localhost:/Applications/

spawn:
	@${cc} -arch ${arch} spawn_cli.c -o spawn_cli
	@/usr/bin/codesign --force --sign - --entitlements entitlements.plist spawn_cli
	@scp -r -P 2222 ./spawn_cli root@localhost:/Applications/

all: main spawn

clean:
	@-rm -rf ./main ./spawn_cli
