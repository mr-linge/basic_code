# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.20)

# 项目信息
project(cmake_study VERSION 1.0.0 LANGUAGES C)

# 查找目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src DIR_SRCS)

# 需要链接的依赖库路径(需要是绝对路径)
link_libraries(/Users/lzd/Repository/basic_code/cmake/study03/src/math/libmymath.dylib)

# 源文件生成可执行文件
add_executable(main ${DIR_SRCS})
