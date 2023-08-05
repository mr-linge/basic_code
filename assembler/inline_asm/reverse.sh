#!/bin/sh
base_dir=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/darwin-x86_64/bin
readelf=${base_dir}/llvm-readelf
objdump=${base_dir}/llvm-objdump
# export readelf=${base_dir}/llvm-readelf
# export objdump=${base_dir}/llvm-objdump

# ${readelf} -a main
${objdump} -d main
