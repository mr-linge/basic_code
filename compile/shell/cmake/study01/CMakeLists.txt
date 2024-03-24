# 指定 CMake 的最低版本 3.20
cmake_minimum_required(VERSION 3.20)

# 指定编译器(不指定会使用默认编译器)
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CPP_COMPILER "/usr/bin/g++")


# 项目信息  指定项目名为 cmake_study,项目版本号为 1.0.0,项目源代码用到 C 语言
project(cmake_study VERSION 1.0.0 LANGUAGES C)

# 编译源文件生成可执行文件
add_executable(main src/main.c)
