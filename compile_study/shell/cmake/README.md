# CMake 使用

## 1. 生成 makefile 文件(所有编译出的文件都放在 build 目录,避免与源文件相互干扰)

cmake -B build

## 2. 执行 makefile 里的编译逻辑

cmake --build build

注:
如果 makefile 文件已经生成了,也可以直接到 makefile 所在目录执行 makefile 里的逻辑
cd ./build
make all
