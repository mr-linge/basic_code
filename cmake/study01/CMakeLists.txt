# 指定 CMake 的最低版本 3.24
cmake_minimum_required(VERSION 3.24)

# 项目信息  指定项目名为 hello_cmake，项目版本号为 1.0.0，项目源代码用到 C 语言
project(hello_cmake VERSION 1.0.0 LANGUAGES C)

# 将 main.c 添加到需要构建的可执行文件里
add_executable(hello_cmake src/main.c)
