# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.2)

# 项目信息
project (cmake_study VERSION 1.0.0 LANGUAGES C)

# 查找目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src DIR_SRCS)

# 添加 math 子目录(math 目录会参与接下来的 cmake 编译流程)
add_subdirectory(./src/math)

# 源文件生成可执行文件
add_executable(main ${DIR_SRCS})

# 源文件添加依赖的静态库
target_link_libraries(main mymath)
